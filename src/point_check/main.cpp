#include <geogram/basic/command_line.h>
#include <geogram/basic/command_line_args.h>
#include <geogram/basic/progress.h>

#include <geogram/mesh/mesh.h>
#include <geogram/mesh/mesh_io.h>
#include <geogram/mesh/mesh_repair.h>
#include <geogram/mesh/mesh_geometry.h> // bbox, mesh_area, compute_normals, simple_Laplacian_smooth, set_anisotropy
#include <geogram/mesh/mesh_preprocessing.h> // fill_holes, remove_small_connected_components, remove_small_facets, expand_border
#include <geogram/mesh/mesh_decimate.h>
#include <geogram/mesh/mesh_surface_intersection.h>
#include <geogram/mesh/mesh_tetrahedralize.h>

#include <geogram/voronoi/CVT.h>
#include <geogram/voronoi/RVD_mesh_builder.h>

#include <fstream>
#include <sstream>
#include <vector>


void initialize_domain_mesh(const GEO::vec3& min, const GEO::vec3& max, GEO::Mesh& domain_mesh)
{
    domain_mesh.clear();
    domain_mesh.vertices.set_dimension(3);

    domain_mesh.vertices.create_vertex(GEO::vec3(min.x, min.y, min.z).data());
    domain_mesh.vertices.create_vertex(GEO::vec3(min.x, min.y, max.z).data());
    domain_mesh.vertices.create_vertex(GEO::vec3(min.x, max.y, min.z).data());
    domain_mesh.vertices.create_vertex(GEO::vec3(min.x, max.y, max.z).data());
    domain_mesh.vertices.create_vertex(GEO::vec3(max.x, min.y, min.z).data());
    domain_mesh.vertices.create_vertex(GEO::vec3(max.x, min.y, max.z).data());
    domain_mesh.vertices.create_vertex(GEO::vec3(max.x, max.y, min.z).data());
    domain_mesh.vertices.create_vertex(GEO::vec3(max.x, max.y, max.z).data());
    
    domain_mesh.facets.create_quad(7,6,2,3);
    domain_mesh.facets.create_quad(1,3,2,0);
    domain_mesh.facets.create_quad(5,7,3,1);
    domain_mesh.facets.create_quad(4,6,7,5);
    domain_mesh.facets.create_quad(4,5,1,0);
    domain_mesh.facets.create_quad(6,4,0,2);

    domain_mesh.facets.connect();
}

void remove_small_components(GEO::Mesh& mesh, double min_comp_area)
{
    if(min_comp_area == 0.0) return;
    GEO::index_t nb_f_removed = mesh.facets.nb();
    GEO::remove_small_connected_components(mesh, min_comp_area);
    nb_f_removed -= mesh.facets.nb();
    if(nb_f_removed != 0)
    {
        double radius = GEO::bbox_diagonal(mesh);
        double epsilon = GEO::CmdLine::get_arg_percent("pre:epsilon", radius);
        GEO::mesh_repair(mesh, GEO::MESH_REPAIR_DEFAULT, epsilon);
    }
}

bool preprocess(GEO::Mesh& mesh)
{
    GEO::Logger::div("preprocessing");
    bool pre = GEO::CmdLine::get_arg_bool("pre");

    double radius = GEO::bbox_diagonal(mesh);

    if(pre)
    {
        double area = GEO::Geom::mesh_area(mesh, 3);
        double epsilon = GEO::CmdLine::get_arg_percent("pre:epsilon", radius);
        double max_area = GEO::CmdLine::get_arg_percent("pre:max_hole_area", area);

        GEO::index_t max_edges = GEO::CmdLine::get_arg_uint("pre:max_hole_edges");
        GEO::index_t nb_bins = GEO::CmdLine::get_arg_uint("pre:vcluster_bins");

        if (nb_bins != 0)
        {
            GEO::mesh_decimate_vertex_clustering(mesh, nb_bins);
        }
        else if(GEO::CmdLine::get_arg_bool("pre:intersect"))
        {
            bool remove_internal_shells = GEO::CmdLine::get_arg_bool("pre:remove_internal_shells");

            GEO::mesh_repair(mesh, GEO::MESH_REPAIR_DEFAULT, epsilon);
            if(max_area != 0.0 && max_edges != 0)
            {
                GEO::fill_holes(mesh, max_area, max_edges);
            }
            GEO::MeshSurfaceIntersection intersection(mesh);
            intersection.set_verbose(GEO::CmdLine::get_arg_bool("sys:verbose"));
            intersection.set_radial_sort(remove_internal_shells);
            intersection.intersect();
            if(remove_internal_shells)
            {
                intersection.remove_internal_shells();
            }
            GEO::mesh_repair(mesh, GEO::MESH_REPAIR_DEFAULT, epsilon);
        }
        else if(GEO::CmdLine::get_arg_bool("pre:repair"))
        {
            GEO::MeshRepairMode mode = GEO::MESH_REPAIR_DEFAULT;
            GEO::mesh_repair(mesh, mode, epsilon);
        }

        remove_small_components(mesh, GEO::CmdLine::get_arg_percent("pre:min_comp_area", area));

        if(!GEO::CmdLine::get_arg_bool("pre:intersect"))
        {
            if(max_area != 0.0 && max_edges != 0)
            {
                GEO::fill_holes(mesh, max_area, max_edges);
            }
        }
    }

    double anisotropy = 0.02 * GEO::CmdLine::get_arg_double("remesh:anisotropy");
    if(anisotropy != 0.0)
    {
        GEO::compute_normals(mesh);
        GEO::index_t nb_normal_smooth = GEO::CmdLine::get_arg_uint("pre:Nsmooth_iter");
        if(nb_normal_smooth != 0)
        {
            GEO::Logger::out("Nsmooth") << "Smoothing normals, " << nb_normal_smooth << " iteration(s)" << std::endl;
            GEO::simple_Laplacian_smooth(mesh, GEO::index_t(nb_normal_smooth), true);
        }
        GEO::set_anisotropy(mesh, anisotropy);
    }

    if(GEO::CmdLine::get_arg_bool("remesh"))
    {
        GEO::index_t nb_removed = mesh.facets.nb();
        GEO::remove_small_facets(mesh, 1e-30);
        nb_removed -= mesh.facets.nb();
        if(nb_removed == 0)
        {
            GEO::Logger::out("Validate") << "Mesh does not have 0-area facets (good)" << std::endl;
        }
        else
        {
            GEO::Logger::out("Validate") << "Removed " << nb_removed << " 0-area facets" << std::endl;
        }
    }

    double margin = GEO::CmdLine::get_arg_percent("pre:margin", radius);
    if(pre && margin != 0.0)
    {
        GEO::expand_border(mesh, margin);
    }

    if(mesh.facets.nb() == 0)
    {
        GEO::Logger::warn("Preprocessing") << "After pre-processing, got an empty mesh" << std::endl;
    }

    return true;
}

int main(int argc, char **argv)
{
    GEO::initialize();

    GEO::CmdLine::import_arg_group("standard");
    GEO::CmdLine::import_arg_group("algo");
    GEO::CmdLine::import_arg_group("poly");
    GEO::CmdLine::import_arg_group("pre");
    GEO::CmdLine::set_arg("pre:repair", true);
    GEO::CmdLine::set_arg("pre:epsilon", 1e-3);
    GEO::CmdLine::import_arg_group("remesh");

    GEO::Mesh M_domain, M_result;
    GEO::vec3 domain_min(-5, -4, 0);
    GEO::vec3 domain_max(15,  4, 8);

    GEO::vector<double> points;
    std::string input_filepath = std::string(DATA_PATH) + "/points.pts";
    std::ifstream inputFile(input_filepath);
    if (inputFile.is_open())
    {
        std::string line;
        while (std::getline(inputFile, line))
        {
            std::stringstream ss(line);
            double num;

            while (ss >> num)
            {
                points.push_back(num);
            }
        }

        inputFile.close();
    }
    else
    {
        GEO::Logger::err("Failed to open the file");
    }

    initialize_domain_mesh(domain_min, domain_max, M_domain);

    if (!preprocess(M_domain))
    {
        GEO::Logger::err("Failed while preprocess");
        exit(1);
    }

    if (!GEO::mesh_tetrahedralize(M_domain))
    {
        GEO::Logger::err("Failed to tetrahedralize");
        exit(1);
    }
    M_domain.cells.compute_borders();

    GEO::Logger::div("CVT");
    GEO::CentroidalVoronoiTesselation CVT(&M_domain, GEO::coord_index_t(3));
    CVT.set_volumetric(true);
    CVT.delaunay()->set_vertices(points.size()/3, points.data());
    CVT.RVD()->set_exact_predicates(true);

    {
        GEO::BuildRVDMesh callback(M_result);
        callback.set_simplify_voronoi_facets(true);

        double angle_threshold = GEO::CmdLine::get_arg_double("poly:normal_angle_threshold");
        if (angle_threshold != 0.0) callback.set_simplify_boundary_facets(true, angle_threshold);

        callback.set_tessellate_non_convex_facets(GEO::CmdLine::get_arg_bool("poly:tessellate_non_convex_facets"));
        callback.set_shrink(GEO::CmdLine::get_arg_double("poly:cells_shrink"));
        callback.set_generate_ids(true);
        CVT.RVD()->for_each_polyhedron(callback);

        GEO::Logger::out("Building RVD done");
    }

    {
        std::string result_output = std::string(RESULT_PATH) + "/result.obj";
        GEO::MeshIOFlags flags;
        flags.set_attributes(GEO::MESH_ALL_ATTRIBUTES);
        GEO::mesh_save(M_result, result_output, flags);
    }
}

project(point_check LANGUAGES CXX)

add_executable(${PROJECT_NAME})

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# SOURCE FILES ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
file(GLOB_RECURSE CXX_SOURCES *.cpp)
set(CXX_FILES ${CXX_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE ${CXX_FILES})

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# LIBRARIES :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
target_link_libraries(${PROJECT_NAME} PRIVATE geogram)
target_include_directories(${PROJECT_NAME} PRIVATE ${COMMON_INCLUDE_DIRS})

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# COMPILER OPTIONS ::::::::::::::::::::::::::::::::::::::::::::::::::::::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

target_compile_definitions(${PROJECT_NAME} PUBLIC
$<$<CONFIG:Debug>:DEBUG>
$<$<NOT:$<CONFIG:Debug>>:NDEBUG>
DATA_PATH="${COMMON_DATA_DIRS}"
RESULT_PATH="${COMMON_OUTPUT_DIRS}"
NOMINMAX
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_compile_options(${PROJECT_NAME} PUBLIC
    # warnings
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic>
    $<$<CXX_COMPILER_ID:GCC>:-pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Werror -Wno-unused>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>

    # flags for each build type
    $<$<CXX_COMPILER_ID:Clang>:
        $<$<CONFIG:Release>:-O3>                  # Release
        $<$<CONFIG:Debug>:-O0 -g>                 # Debug
        $<$<CONFIG:RelWithDebgInfo>:-O3 -g>       # RelWithDebInfo
    >
    $<$<CXX_COMPILER_ID:GCC>:
        $<$<CONFIG:Release>:-O3>                  # Release
        $<$<CONFIG:Debug>:-O0 -g>                 # Debug
        $<$<CONFIG:RelWithDebgInfo>:-O3 -g>       # RelWithDebInfo
    >

    $<$<CXX_COMPILER_ID:MSVC>: 
        $<$<CONFIG:Release>:/O2 /Zi /MP>          # Release
        $<$<CONFIG:Debug>:/Od /ZI /MP>            # Debug
        $<$<CONFIG:RelWithDebgInfo>:/O2 /ZI /MP>  # RelWithDebInfo
    >
)
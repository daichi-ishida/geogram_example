cmake_minimum_required(VERSION 3.17)

project(example)

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# GLOBAL SETTINGS :::::::::::::::::::::::::::::::::::::::::::::::::::::::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

# make a bin folder to host all the executables
make_directory(${PROJECT_SOURCE_DIR}/bin)
make_directory(${PROJECT_SOURCE_DIR}/bin/debug)
make_directory(${PROJECT_SOURCE_DIR}/bin/release)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/release")

set(COMMON_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}/common")
set(COMMON_DATA_DIRS "${PROJECT_SOURCE_DIR}/data")
set(COMMON_OUTPUT_DIRS "${PROJECT_SOURCE_DIR}/output")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# OPTIONS :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# geogram config
option(GEOGRAM_WITH_GRAPHICS "Viewers and geogram_gfx library" OFF)
option(GEOGRAM_LIB_ONLY "Libraries only (no example programs/no viewer)" ON)
option(GEOGRAM_WITH_VORPALINE "with vorpaline" OFF)

# add geogram
add_subdirectory("geogram")

#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
# SUBDIRECTORIES ::::::::::::::::::::::::::::::::::::::::::::::::::::::::
#::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
add_subdirectory("src")

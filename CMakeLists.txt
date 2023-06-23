cmake_minimum_required(VERSION 3.15)
project(FFGL-SDK
    VERSION 2.2
    LANGUAGES CXX
)

# resolve dependencies if using vcpkg
if (EXISTS CACHE{VCPKG_MANIFEST_FILE})
    vcpkg_acquire_dependencies()
endif()

add_library(ffgl_sdk STATIC)
add_library(ffgl::sdk ALIAS ffgl_sdk)
target_sources(ffgl_sdk PUBLIC source/lib/FFGLSDK.h)
target_sources(ffgl_sdk PRIVATE source/lib/FFGLSDK.cpp)

target_include_directories(ffgl_sdk PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/source/lib/")

# we need glew (except on macOS)
if (NOT APPLE)
    find_package(GLEW REQUIRED)
    target_link_libraries(ffgl_sdk PRIVATE GLEW::GLEW)
endif()

# are we building ffgl as the main repo, or is it included
# into another project?
set(FFGL_MASTER_PROJECT OFF)
if ("${CMAKE_CURRENT_SOURCE_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
    set(FFGL_MASTER_PROJECT ON)
endif()

# should we build the example plugins? we do this by default
# if building the project directly, but not when adding ffgl
# to another project
option(FFGL_BUILD_EXAMPLE_PLUGINS ${FFGL_MASTER_PROJECT} "Build the example FFGL plugins")

if (${FFGL_BUILD_EXAMPLE_PLUGINS})
    add_subdirectory(source/plugins)
endif()

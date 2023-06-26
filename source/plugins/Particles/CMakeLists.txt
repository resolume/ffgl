add_library(ffgl_plugin_particles STATIC)
add_library(ffgl::plugin::particles ALIAS ffgl_plugin_particles)
target_sources(ffgl_plugin_particles PRIVATE
    Constants.h     Constants.cpp
    GLResources.h   GLResources.cpp
    Particles.h     Particles.cpp
    shaders/fsRender.h
    shaders/gsRender.h
    shaders/vsRender.h
    shaders/vsUpdate.h
)
target_link_libraries(ffgl_plugin_particles PRIVATE ffgl::sdk)

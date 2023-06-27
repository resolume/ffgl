add_library(ffgl-plugin-particles STATIC)
add_library(ffgl::plugin::particles ALIAS ffgl-plugin-particles)
target_sources(ffgl-plugin-particles PRIVATE
    Constants.h     Constants.cpp
    GLResources.h   GLResources.cpp
    Particles.h     Particles.cpp
    shaders/fsRender.h
    shaders/gsRender.h
    shaders/vsRender.h
    shaders/vsUpdate.h
)
target_link_libraries(ffgl-plugin-particles PRIVATE ffgl::sdk)

install(
    TARGETS     ffgl-plugin-particles
    EXPORT      ffgl--plugin-particles-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

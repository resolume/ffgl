add_library(ffgl-plugin-events STATIC)
add_library(ffgl::plugin::events ALIAS ffgl-plugin-events)
target_sources(ffgl-plugin-events PRIVATE FFGLEvents.h FFGLEvents.cpp)
target_link_libraries(ffgl-plugin-events PRIVATE ffgl::sdk)

install(
    TARGETS     ffgl-plugin-events
    EXPORT      ffgl--plugin-events-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

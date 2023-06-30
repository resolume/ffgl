add_library(ffgl-plugin-gradients STATIC)
add_library(ffgl::plugin::gradients ALIAS ffgl-plugin-gradients)
target_sources(ffgl-plugin-gradients PRIVATE FFGLGradients.h FFGLGradients.cpp)
target_link_libraries(ffgl-plugin-gradients PRIVATE ffgl::sdk)

install(
    TARGETS     ffgl-plugin-gradients
    EXPORT      ffgl--plugin-gradients-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

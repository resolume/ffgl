add_library(ffgl_plugin_gradients STATIC)
add_library(ffgl::plugin::gradients ALIAS ffgl_plugin_events)
target_sources(ffgl_plugin_gradients PRIVATE FFGLGradients.h FFGLGradients.cpp)
target_link_libraries(ffgl_plugin_gradients PRIVATE ffgl::sdk)

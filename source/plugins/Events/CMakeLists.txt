add_library(ffgl_plugin_events STATIC)
add_library(ffgl::plugin::events ALIAS ffgl_plugin_events)
target_sources(ffgl_plugin_events PRIVATE FFGLEvents.h FFGLEvents.cpp)
target_link_libraries(ffgl_plugin_events PRIVATE ffgl::sdk)

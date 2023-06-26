add_library(ffgl_plugin_add_subtract STATIC)
add_library(ffgl::plugin::add_subtract ALIAS ffgl_plugin_add_subtract)
target_sources(ffgl_plugin_add_subtract PRIVATE AddSubtract.h AddSubtract.cpp)
target_link_libraries(ffgl_plugin_add_subtract PRIVATE ffgl::sdk)

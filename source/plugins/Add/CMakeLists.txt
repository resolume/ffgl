add_library(ffgl_plugin_add STATIC)
add_library(ffgl::plugin::add ALIAS ffgl_plugin_add)
target_sources(ffgl_plugin_add PRIVATE Add.h Add.cpp)
target_link_libraries(ffgl_plugin_add PRIVATE ffgl::sdk)

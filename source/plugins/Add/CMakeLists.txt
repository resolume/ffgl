add_library(ffgl-plugin-add STATIC)
add_library(ffgl::plugin::add ALIAS ffgl-plugin-add)
target_sources(ffgl-plugin-add PRIVATE Add.h Add.cpp)
target_link_libraries(ffgl-plugin-add PRIVATE ffgl::sdk)

install(
    TARGETS     ffgl-plugin-add
    EXPORT      ffgl-plugin-add-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

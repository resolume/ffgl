add_library(ffgl-plugin-add-subtract STATIC)
add_library(ffgl::plugin::add_subtract ALIAS ffgl-plugin-add-subtract)
target_sources(ffgl-plugin-add-subtract PRIVATE AddSubtract.h AddSubtract.cpp)
target_link_libraries(ffgl-plugin-add-subtract PRIVATE ffgl::sdk)

install(
    TARGETS     ffgl-plugin-add-subtract
    EXPORT      ffgl--plugin-add-subtract-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

add_library(ffgl-plugin-custom-thumbnails STATIC)
add_library(ffgl::plugin::custom-thumbnails ALIAS ffgl-plugin-custom-thumbnails)

target_sources(ffgl-plugin-custom-thumbnails PRIVATE
    CustomThumbnail.h   CustomThumbnail.cpp
    PNGLoader.h         PNGLoader.cpp
    Thumb.h
)

find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

target_link_libraries(ffgl-plugin-custom-thumbnails PRIVATE PNG::PNG)
target_link_libraries(ffgl-plugin-custom-thumbnails PRIVATE ZLIB::ZLIB)

target_link_libraries(ffgl-plugin-custom-thumbnails PRIVATE ffgl::sdk)
target_link_libraries(ffgl-plugin-custom-thumbnails PRIVATE png_static)

install(
    TARGETS     ffgl-plugin-custom-thumbnails
    EXPORT      ffgl--plugin-custom-thumbnails-targets
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

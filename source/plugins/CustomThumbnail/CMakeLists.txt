add_library(ffgl_plugin_custom_thumbnails STATIC)
add_library(ffgl::plugin::custom_thumbnails ALIAS ffgl_plugin_custom_thumbnails)

target_sources(ffgl_plugin_custom_thumbnails PRIVATE
    CustomThumbnail.h   CustomThumbnail.cpp
    PNGLoader.h         PNGLoader.cpp
    Thumb.h
)

find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

target_link_libraries(ffgl_plugin_custom_thumbnails PRIVATE PNG::PNG)
target_link_libraries(ffgl_plugin_custom_thumbnails PRIVATE ZLIB::ZLIB)

target_link_libraries(ffgl_plugin_custom_thumbnails PRIVATE ffgl::sdk)
target_link_libraries(ffgl_plugin_custom_thumbnails PRIVATE png_static)

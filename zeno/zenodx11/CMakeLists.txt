add_library(zenodx11)

target_link_libraries(
    zenodx11
    PUBLIC
    d3d11.lib
    dxgi.lib
)

target_compile_definitions(
    zenodx11
    PUBLIC
    ZENO_LIB
)

target_include_directories(
    zenodx11
    PRIVATE
    "../"
)

target_sources(
    zenodx11
    PUBLIC
    "sources/renderer.cpp"
)
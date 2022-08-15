add_library(zenodx11)

target_compile_definitions(
    zenodx11
    PUBLIC
    ZENO_LIB
)

target_link_libraries(
    zenodx11
    PUBLIC
    d3d11.lib
)

target_include_directories(
    zenodx11
    PRIVATE
    "../"
)

target_sources(
    zenodx11
    PUBLIC
    "source/renderer.cpp"
)
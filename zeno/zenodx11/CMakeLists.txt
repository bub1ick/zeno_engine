add_library(zenodx11)

# provide access to zenosys to dependants of zenodx11
target_link_libraries(
    zenodx11
    PUBLIC
    zenosys
    zenoutils
    DXGI.lib
    D3D11.lib
)

# define needed macro
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
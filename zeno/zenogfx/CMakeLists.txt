add_library(zenogfx)

# provide access to zenosys to dependants of zenodx11
target_link_libraries(
    zenogfx
    PUBLIC
    zenosys
    zenoutils
    DXGI.lib
    D3D11.lib
    D3DCompiler.lib
)

# define needed macro
target_compile_definitions(
    zenogfx
    PUBLIC
    ZENO_LIB
)

target_include_directories(
    zenogfx
    PRIVATE
    "../"
)

target_sources(
    zenogfx
    PUBLIC
    "sources/renderer.cpp"
    "sources/components/dxgi.cpp"
    "sources/components/dx11.cpp"
    "sources/exceptions.cpp"
)
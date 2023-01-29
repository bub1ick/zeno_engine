add_library(zeno)

target_compile_definitions(
    zeno
    PUBLIC
    ZENO_LIB
)

target_link_libraries(
    zeno
    PUBLIC
    zenoutils
    zenosys
    zenogfx
)

# Add include directory to be able to use engine's primary include file (zeno.hpp)
target_include_directories(
    zeno
    PRIVATE
    "../"
)

target_sources(
    zeno
    PUBLIC
    "source/engine.cpp"
    "../resources/internal/zeno.manifest"
)
add_library(zeno STATIC)

target_compile_definitions(
    zeno
    PUBLIC
    ZENO_LIB
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
)
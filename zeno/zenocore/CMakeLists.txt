add_library(zenocore STATIC)

target_link_libraries(
    zenocore
    PUBLIC
    SDL2.lib
)

# Add include directory to be able to use engine's primary include file (zeno.hpp)
target_include_directories(
    zenocore
    PRIVATE
    "../"
)

target_sources(
    zenocore
    PUBLIC
    "source/engine.cpp"
)
add_library(zenocore STATIC)

# Add include directory to be able to use engine's primary include file (zeno.hpp)
target_include_directories(
    zenocore
    PRIVATE
    "../"
)

target_link_libraries(zenocore d3d11.lib)

target_sources(
    zenocore
    PUBLIC
    "source/application/application.cpp"
    "source/application/window.cpp"
)
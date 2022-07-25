add_library(zenocore STATIC)

# Add include directory to be able to use engine's primary include file (zeno.hpp)
target_include_directories(
    zenocore
    PRIVATE
    "../"
)

target_sources(
    zenocore
    PUBLIC
    "source/application/application.cpp"
    "source/application/window.cpp"
)
add_library(zenocore)

target_link_libraries(
    zenocore
    PUBLIC
    zenodx11
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
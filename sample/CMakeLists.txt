# Declare sample executable utilizing the zeno engine.
add_executable(sample WIN32)

# State which projects our executable depends on for correct order of compilation.
add_dependencies(sample zenocore)

target_link_libraries(
    sample
    PRIVATE
    zenocore
)

# Add include directory to be able to use engine's primary include file (zeno.hpp)
target_include_directories(
    sample
    PRIVATE
    "${PROJECT_SOURCE_DIR}/zeno/"
)

# Add .cpp files for the executable
target_sources(
    sample
    PRIVATE
    "source/main.cpp"
)
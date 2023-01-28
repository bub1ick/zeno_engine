# Declare sample executable utilizing the zeno engine.
add_executable(sample WIN32)

target_link_libraries(
    sample
    PUBLIC
    zeno
    zenogfx
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
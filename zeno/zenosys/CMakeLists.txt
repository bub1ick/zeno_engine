add_library(zenosys STATIC)

target_link_libraries(
    zenosys
    PUBLIC
)

target_compile_definitions(
    zenosys
    PUBLIC
    ZENO_LIB
)

target_include_directories(
    zenosys
    PRIVATE
    "../"
)

target_sources(
    zenosys
    PUBLIC
    "sources/window.cpp"
)
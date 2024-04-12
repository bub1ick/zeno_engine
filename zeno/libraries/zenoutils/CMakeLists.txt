add_library(zenoutils STATIC)

target_compile_definitions(
    zenoutils
    PUBLIC
    ZENO_LIB
)

target_include_directories(
    zenoutils
    PRIVATE
    "../../"
)

target_sources(
    zenoutils
    PUBLIC
    "sources/string.cpp"
)
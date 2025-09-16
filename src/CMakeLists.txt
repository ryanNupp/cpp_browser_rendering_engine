cmake_minimum_required(VERSION 3.25)

add_library(render_engine STATIC)

target_compile_features(render_engine PRIVATE cxx_std_23)

target_include_directories(render_engine PUBLIC "${PROJECT_SOURCE_DIR}/src")

target_sources(render_engine PUBLIC
    
)
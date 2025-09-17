cmake_minimum_required(VERSION 3.25)

project(
    browser_render_demo
    #
    LANGUAGES CXX
    VERSION 0.0.1
)


# -------- Executable --------

add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE app/main.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    #
    CMAKE_EXPORT_COMPILE_COMMANDS ON
)


# -------- Libraries --------

add_subdirectory(src)
target_link_libraries(${PROJECT_NAME} PRIVATE render_engine)
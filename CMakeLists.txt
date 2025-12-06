cmake_minimum_required(VERSION 3.28)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Wextra -pedantic-errors -Werror)

project(http_server VERSION 1.0 LANGUAGES C CXX)

add_executable(http_server src/main.cpp)
target_include_directories(http_server PRIVATE include)

set_property(TARGET http_server PROPERTY CXX_STANDARD 17)
set_property(TARGET http_server PROPERTY CXX_STANDARD_REQUIRED ON)

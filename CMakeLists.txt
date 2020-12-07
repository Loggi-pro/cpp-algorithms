cmake_minimum_required(VERSION 3.16)
project(cpp_algorithms)

set(CMAKE_CXX_STANDARD 17)

file(GLOB test_src "tests/*.cpp")

add_executable(cpp_algorithms src/main.cpp ${test_src})

target_include_directories(cpp_algorithms PUBLIC src libs)
# settings
add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)
# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)
# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

include(cmake/Conan.cmake)
run_conan()


target_link_libraries(cpp_algorithms PRIVATE project_options project_warnings CONAN_PKG::gtest)

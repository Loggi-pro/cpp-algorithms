cmake_minimum_required(VERSION 3.16)
project(cpp_algorithms)
include(cmake/Conan.cmake)
SET(CONANFILE ${CMAKE_SOURCE_DIR}/conanfile.txt)
run_conan()

# settings
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
add_library(project_warnings INTERFACE)
# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)
# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

enable_testing()
add_subdirectory(tests)



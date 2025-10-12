include(GoogleTest)
find_package(gtest CONFIG REQUIRED)

file(GLOB_RECURSE test_src "*.cpp")

add_executable(tests ${test_src})

target_include_directories(tests PUBLIC ../src)
target_link_libraries(tests PUBLIC project_options project_warnings gtest::gtest)
gtest_discover_tests(tests)
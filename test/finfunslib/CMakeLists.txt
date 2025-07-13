add_executable(finfunslib_tests main.cpp)

target_link_libraries(finfunslib_tests PRIVATE finfuns::lib doctest::doctest)

add_test(NAME finfunslib_tests COMMAND finfunslib_tests)

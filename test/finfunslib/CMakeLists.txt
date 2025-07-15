add_executable(finfunslib_tests main.cpp)

target_link_libraries(finfunslib_tests PRIVATE finfuns::lib doctest::doctest)

# We reuse some of the test data - which needs some headers
target_link_libraries(finfunslib_tests PRIVATE finfuns::headers)

add_test(NAME finfunslib_tests COMMAND finfunslib_tests)

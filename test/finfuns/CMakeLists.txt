add_executable(finfuns_tests main.cpp irr_test.cpp npv_test.cpp xnpv_test.cpp)

target_link_libraries(finfuns_tests PRIVATE finfuns::finfuns doctest::doctest)

add_test(NAME finfuns_tests COMMAND finfuns_tests)

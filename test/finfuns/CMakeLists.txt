add_executable(
    finfuns_tests
    main.cpp
    fv_test.cpp
    irr_test.cpp
    npv_test.cpp
    pv_test.cpp
    xirr_test.cpp
    xnpv_test.cpp
)

target_link_libraries(finfuns_tests PRIVATE finfuns::headers)

target_include_directories(
    finfuns_tests
    SYSTEM
    PRIVATE $<TARGET_PROPERTY:doctest::doctest,INTERFACE_INCLUDE_DIRECTORIES>
)

add_test(NAME finfuns_tests COMMAND finfuns_tests)

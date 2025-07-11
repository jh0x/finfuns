add_executable(ex_irr ex_irr.cpp)
target_link_libraries(ex_irr PRIVATE finfuns::headers)

add_executable(ex_npv ex_npv.cpp)
target_link_libraries(ex_npv PRIVATE finfuns::headers)

add_executable(ex_xirr ex_xirr.cpp)
target_link_libraries(ex_xirr PRIVATE finfuns::headers)

add_executable(ex_xnpv ex_xnpv.cpp)
target_link_libraries(ex_xnpv PRIVATE finfuns::headers)

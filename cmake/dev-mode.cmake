include(cmake/folders.cmake)

include(CTest)
if(BUILD_TESTING)
    add_library(doctest INTERFACE)
    target_include_directories(
        doctest
        INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/extern/doctest
    )
    add_library(doctest::doctest ALIAS doctest)

    add_subdirectory(test)
endif()

add_folders(Project)

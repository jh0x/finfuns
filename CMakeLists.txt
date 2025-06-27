cmake_minimum_required(VERSION 3.20)
project(finfuns VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(finfuns INTERFACE)
add_library(finfuns::finfuns ALIAS finfuns)

target_compile_features(finfuns INTERFACE cxx_std_23)

target_include_directories(finfuns INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

option(FINFUNS_BUILD_TESTS "Build tests for finfuns" OFF)

if(FINFUNS_BUILD_TESTS)
    enable_testing()

    add_library(doctest INTERFACE)
    target_include_directories(doctest INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/extern/doctest)
    add_library(doctest::doctest ALIAS doctest)
    
    add_subdirectory(test)
endif()

include(GNUInstallDirs)
install(DIRECTORY include/finfuns DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS finfuns EXPORT finfuns-targets)
install(EXPORT finfuns-targets
    FILE finfuns-config.cmake
    NAMESPACE finfuns::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/finfuns
)
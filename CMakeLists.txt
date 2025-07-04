cmake_minimum_required(VERSION 3.20)

include(cmake/prelude.cmake)

project(finfuns VERSION 0.1.0 LANGUAGES CXX)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Dependencies ----

find_package(Boost 1.68.0 REQUIRED COMPONENTS headers)
message(STATUS "Boost found: ${Boost_VERSION}")
message(STATUS "Boost includes: ${Boost_INCLUDE_DIRS}")

# ---- Declare library ----

add_library(finfuns INTERFACE)
add_library(finfuns::finfuns ALIAS finfuns)

set_property(TARGET finfuns PROPERTY EXPORT_NAME finfuns)

target_compile_features(finfuns INTERFACE cxx_std_23)

target_include_directories(
    finfuns
    ${warning_guard}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${Boost_INCLUDE_DIRS}>
)
target_link_libraries(finfuns INTERFACE Boost::headers)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
    include(cmake/install-rules.cmake)
endif()

option(finfuns_BUILD_EXAMPLES "Build examples" OFF)

if(PROJECT_IS_TOP_LEVEL AND finfuns_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# ---- Developer mode ----
if(NOT finfuns_DEVELOPER_MODE)
    return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
    message(
        AUTHOR_WARNING
        "Developer mode is intended for developers of finfuns"
    )
endif()

include(cmake/dev-mode.cmake)

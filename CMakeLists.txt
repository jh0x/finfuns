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

find_package(Boost REQUIRED CONFIG COMPONENTS headers)

# ---- Declare header-only library ----

add_library(finfuns INTERFACE)
add_library(finfuns::headers ALIAS finfuns)

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

# ---- Declare library ----
add_library(finfunslib lib/finfunslib/src/finfunslib.cpp)
add_library(finfuns::lib ALIAS finfunslib)

include(GenerateExportHeader)
generate_export_header(
    finfunslib
    BASE_NAME finfunslib
    EXPORT_FILE_NAME export/shared/finfunslib_export.h
    CUSTOM_CONTENT_FROM_VARIABLE pragma_suppress_c4251
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(finfunslib PUBLIC SHARED_STATIC_DEFINE)
endif()
if(WIN32 AND BUILD_SHARED_LIBS)
    target_compile_definitions(finfunslib PRIVATE "FINFUNSLIB_BUILD_DLL")
endif()

set_target_properties(
    finfunslib
    PROPERTIES
        CXX_VISIBILITY_PRESET hidden
        VISIBILITY_INLINES_HIDDEN YES
        VERSION "${PROJECT_VERSION}"
        SOVERSION "${PROJECT_VERSION_MAJOR}"
        EXPORT_NAME finfunslib
        OUTPUT_NAME finfunslib
)

target_include_directories(
    finfunslib
    SYSTEM
    PUBLIC
        "\$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/>"
        "\$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/export>"
)

target_link_libraries(finfunslib PRIVATE finfuns::headers)

target_compile_features(finfunslib PUBLIC cxx_std_23)

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

if(PROJECT_IS_TOP_LEVEL)
    set(CMAKE_INSTALL_INCLUDEDIR
        "include/finfuns-${PROJECT_VERSION}"
        CACHE STRING
        ""
    )
endif()

set(CMAKE_INSTALL_LIBDIR lib CACHE PATH "")

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Define where CMake config files will be installed
set(finfuns_INSTALL_CMAKEDIR
    "${CMAKE_INSTALL_DATADIR}/finfuns"
    CACHE STRING
    "CMake package config location relative to the install prefix"
)

set(package finfuns) # Reusable variable

# Install headers
install(
    DIRECTORY include/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    COMPONENT finfuns_Development
)

# Install header-only target
install(
    TARGETS finfuns
    EXPORT finfunsTargets
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Install compiled library (static/shared)
install(
    TARGETS finfunslib
    EXPORT finfunsTargets
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT finfuns_Runtime
)

# Install export header
install(
    FILES "${PROJECT_BINARY_DIR}/export/shared/finfunslib_export.hpp"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/finfuns"
    COMPONENT finfuns_Development
)

# Generate and install config files
write_basic_package_version_file(
    "${package}ConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

install(
    FILES cmake/install-config.cmake
    DESTINATION "${finfuns_INSTALL_CMAKEDIR}"
    RENAME "${package}Config.cmake"
)

install(
    FILES "${PROJECT_BINARY_DIR}/${package}ConfigVersion.cmake"
    DESTINATION "${finfuns_INSTALL_CMAKEDIR}"
)

install(
    EXPORT finfunsTargets
    NAMESPACE finfuns::
    DESTINATION "${finfuns_INSTALL_CMAKEDIR}"
)

if(PROJECT_IS_TOP_LEVEL)
    include(CPack)
endif()

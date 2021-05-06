##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================

##==================================================================================================
## Install process
##==================================================================================================
include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/kiwaku)

install(TARGETS kiwaku
    EXPORT kiwaku-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

##==================================================================================================
## This is required so that the exported target has the name KIWAKU and not kiwaku
##==================================================================================================
set_target_properties(kiwaku PROPERTIES EXPORT_NAME kiwaku)

install ( DIRECTORY ${PROJECT_SOURCE_DIR}/include/kiwaku
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )

##==================================================================================================
## Export the targets to a script
##==================================================================================================
install(EXPORT kiwaku-targets
  FILE
    kiwakuTargets.cmake
  NAMESPACE
    kiwaku::
  DESTINATION
    ${INSTALL_CONFIGDIR}
)

##==================================================================================================
## Create a ConfigVersion.cmake file
##==================================================================================================
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/kiwakuConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/kiwakuConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/kiwakuConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

##==================================================================================================
##Install the config, configversion and custom find modules
##==================================================================================================
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kiwakuConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/kiwakuConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

##==================================================================================================
## Exporting from the build tree
##==================================================================================================
export(EXPORT kiwaku-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/kiwakuTargets.cmake NAMESPACE kiwaku::)

##==================================================================================================
## Register package in user's package registry
##==================================================================================================
export(PACKAGE kiwaku)

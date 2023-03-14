##==================================================================================================
##  EVE - Expressive Vector Engine
##  Copyright : EVE Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
include(GNUInstallDirs)
set(MAIN_DEST     "${CMAKE_INSTALL_LIBDIR}/kiwaku-${PROJECT_VERSION}")
set(INSTALL_DEST  "${CMAKE_INSTALL_INCLUDEDIR}/kiwaku-${PROJECT_VERSION}")
set(DOC_DEST      "${CMAKE_INSTALL_DOCDIR}-${PROJECT_VERSION}")

## =================================================================================================
## Exporting target for external use
## =================================================================================================
add_library(kiwaku_lib INTERFACE)
target_include_directories( kiwaku_lib INTERFACE
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${INSTALL_DEST}>
                          )
target_compile_features(kiwaku_lib INTERFACE cxx_std_20)
set_target_properties(kiwaku_lib PROPERTIES EXPORT_NAME kiwaku)
add_library(kiwaku::kiwaku ALIAS kiwaku_lib)

## =================================================================================================
## Install target with versioned folder
## =================================================================================================
install(TARGETS   kiwaku_lib EXPORT kiwaku-targets DESTINATION "${MAIN_DEST}")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/kwk               DESTINATION "${INSTALL_DEST}" )
install(FILES     ${PROJECT_SOURCE_DIR}/cmake/kiwaku-config.cmake    DESTINATION "${MAIN_DEST}"    )
install(FILES     ${PROJECT_SOURCE_DIR}/LICENSE.md                DESTINATION "${DOC_DEST}"     )
install(EXPORT    kiwaku-targets NAMESPACE "kiwaku::" DESTINATION "${MAIN_DEST}")

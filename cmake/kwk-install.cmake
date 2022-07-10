##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: MIT
##==================================================================================================
set(MAIN_DEST     "lib")
set(INSTALL_DEST  "include")

## =================================================================================================
## Exporting target for external use
## =================================================================================================
add_library(kwk_lib INTERFACE)
target_include_directories( kwk_lib INTERFACE
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${INSTALL_DEST}>
                          )
target_compile_features(kwk_lib INTERFACE cxx_std_20)
add_library(kwk::kwk ALIAS kwk_lib)

## =================================================================================================
## Install target with versionned folder
## =================================================================================================
install(TARGETS   kwk_lib EXPORT kwk_lib  DESTINATION "${MAIN_DEST}")
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/kwk               DESTINATION "${INSTALL_DEST}" )
install(FILES     ${PROJECT_SOURCE_DIR}/cmake/kwk-config.cmake    DESTINATION "${MAIN_DEST}"    )
install(EXPORT    kwk_lib DESTINATION "${MAIN_DEST}")

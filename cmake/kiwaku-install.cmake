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
add_library(kiwaku_lib INTERFACE)
target_include_directories( kiwaku_lib INTERFACE
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${INSTALL_DEST}>
                          )
target_compile_features(kiwaku_lib INTERFACE cxx_std_20)
add_library(kiwaku::kiwaku ALIAS kiwaku_lib)

## =================================================================================================
## Install target with versionned folder
## =================================================================================================
install(TARGETS   kiwaku_lib EXPORT kiwaku_lib                    DESTINATION "${MAIN_DEST}"    )
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/kwk               DESTINATION "${INSTALL_DEST}" )
install(FILES     ${PROJECT_SOURCE_DIR}/cmake/kiwaku-config.cmake DESTINATION "${MAIN_DEST}"    )
install(EXPORT    kiwaku_lib DESTINATION "${MAIN_DEST}")

##======================================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## Download and setup CPM
##======================================================================================================================
set(CPM_DOWNLOAD_VERSION 0.40.8)

if(CPM_SOURCE_CACHE)
  set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
  set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "[${PROJECT_NAME}] Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
  file(DOWNLOAD
       https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
       ${CPM_DOWNLOAD_LOCATION}
  )
endif()

include(${CPM_DOWNLOAD_LOCATION})

##======================================================================================================================
## Retrieve dependencies
##======================================================================================================================
CPMAddPackage(NAME COPACABANA GITHUB_REPOSITORY SylvainJoube/copacabana GIT_TAG main)

if(KIWAKU_BUILD_TEST)
  CPMAddPackage ( NAME TTS   GITHUB_REPOSITORY SylvainJoube/tts
                  GIT_TAG main
                  OPTIONS "TTS_BUILD_TEST OFF"
                          "TTS_BUILD_DOCUMENTATION OFF"
                          "TTS_QUIET ON"
                )
endif()

CPMAddPackage(NAME KUMI   GITHUB_REPOSITORY SylvainJoube/kumi
              GIT_TAG main
              OPTIONS "KUMI_BUILD_TEST OFF"
                      "KUMI_BUILD_DOCUMENTATION OFF"
                      "KUMI_QUIET ON"
              )

CPMAddPackage(NAME RABERU GITHUB_REPOSITORY SylvainJoube/raberu
              GIT_TAG main
              OPTIONS "RABERU_BUILD_TEST OFF"
                      "RABERU_BUILD_DOCUMENTATION OFF"
                      "RABERU_QUIET ON"
              )

CPMAddPackage ( NAME EVE   GITHUB_REPOSITORY SylvainJoube/eve
                GIT_TAG main
                OPTIONS "EVE_BUILD_TEST OFF"
                        "EVE_BUILD_BENCHMARKS OFF"
                        "EVE_BUILD_DOCUMENTATION OFF"
              )


##======================================================================================================================
##  KIWAKU - Shape Up Your Data
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## CPM is vendored: the download bootstrap reports nothing on a failure and leaves an empty file.
##======================================================================================================================
include(${CMAKE_CURRENT_LIST_DIR}/CPM.cmake)

##======================================================================================================================
## Retrieve dependencies
##======================================================================================================================
CPMAddPackage(NAME COPACABANA GITHUB_REPOSITORY jfalcou/copacabana GIT_TAG v8)

CPMDeclarePackage(
  TTS
  NAME TTS
  GITHUB_REPOSITORY jfalcou/tts
  GIT_TAG 4.0
  SYSTEM YES
  OPTIONS "TTS_BUILD_TEST OFF" "TTS_BUILD_DOCUMENTATION OFF" "TTS_QUIET ON")

CPMDeclarePackage(
  KUMI
  NAME KUMI
  GITHUB_REPOSITORY jfalcou/kumi
  GIT_TAG v5.0
  SYSTEM YES
  OPTIONS "KUMI_BUILD_TEST OFF" "KUMI_BUILD_DOCUMENTATION OFF" "KUMI_QUIET ON")

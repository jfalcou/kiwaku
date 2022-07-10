#!/bin/sh -l
set -e

# ID for various tests
INSTALL_TEST=0
FETCH_TEST=1
CPM_TEST=2

if [ $2 -eq $INSTALL_TEST ]
then
  echo "::group::Prepare KIWAKU repository for branch " $1
  mkdir build && cd build
  cmake .. -G Ninja
  ninja install | grep cmake
  CURRENT_SHA1=`git rev-parse HEAD`
  cd ..
  echo "::endgroup::"

  echo "::group::Test KIWAKU via the install target"
  mkdir install-test && cd install-test
  cmake ../test/integration/install-test -G Ninja
  ninja test_kiwaku
  ./test_kiwaku
  echo "::endgroup::"
fi

if [ $2 -eq $FETCH_TEST ]
then
  echo "::group::Test KIWAKU via FetchContent"
  mkdir fetch-test && cd fetch-test
  cmake ../test/integration/fetch-test -G Ninja -DKIWAKU_SHA1=$1
  ninja test_kiwaku
  ./test_kiwaku
  echo "::endgroup::"
fi

if [ $2 -eq $CPM_TEST ]
then
  echo "::group::Test KIWAKU via CPM"
  mkdir cpm-test && cd cpm-test
  cmake ../test/integration/cpm-test -G Ninja -DKIWAKU_SHA1=$1
  ninja test_kiwaku
  ./test_kiwaku
  echo "::endgroup::"
fi

return 0;

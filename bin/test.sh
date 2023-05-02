#!/bin/bash
rm ./test-BbMm
./bin/build.sh

# Test
echo "
  T E S T   B B - M M   C O R E   L I B
  ------------------------------------------------------------------------"
./test-BbMm

echo "
  T E S T   P Y T H O N   W R A P
  -------------------------------"
cd python-src
pytest

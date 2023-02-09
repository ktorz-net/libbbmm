#!/bin/bash
rm ./test-BeMAge
./bin/build.sh

# Test
echo "
  T E S T   W A N D A   C O R E
  -----------------------------"
./test-BeMAge

echo "
  T E S T   P Y T H O N   W R A P
  -------------------------------"
cd python-src
pytest

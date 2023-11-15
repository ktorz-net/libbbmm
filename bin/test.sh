#!/bin/bash
rm ./run-test-BbMm
./bin/build.sh

# Test
echo "
  T E S T   B B - M M   C O R E   L I B
  ------------------------------------------------------------------------"
./run-test-BbMm

echo "
  T E S T   P Y T H O N   W R A P
  -------------------------------"
cd pyBbMm
pytest

#!/bin/bash
rm ./test-BbMmm
./bin/build.sh

# Test
echo "
  T E S T   B a y e s i a n B a s e d - M a r k o v M o d e l s   C O R E
  ------------------------------------------------------------------------"
./test-BbMm

echo "
  T E S T   P Y T H O N   W R A P
  -------------------------------"
cd python-src
pytest

#!/usr/bin/env bash
./clean.sh

mkdir local
cd local
wget -O- https://get.pharo.org/64 | bash
cd ..

./local/pharo local/Pharo.image save Wanda
./local/pharo local/Wanda.image eval --save "Metacello new baseline: 'Wanda'; repository: 'gitlocal://$PWD'; load."
./local/pharo local/Wanda.image save WandaFresh

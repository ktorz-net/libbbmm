#!/bin/bash

# Generate Pharo image with Wanda:
#---------------------------------
bin/createWandaImage.sh

# Generate a launcher in local environment
#-----------------------------------------

WANDAPATH=`sed -e "s/ /\\\ /g" <<<$PWD`

echo "[Desktop Entry]
Name=Wanda
Comment=Wanda
Exec=$WANDAPATH/local/pharo-ui ./local/Wanda.image
Path=$WANDAPATH
Icon=$WANDAPATH/resources/icon.png
Terminal=false
Type=Application
Categories=Development;Pharo
" > test

mv test ~/.local/share/applications/Wanda.desktop

#!/bin/bash
#script de creation de l'image pour OSX
#Eric Seigne 2011 <eric.seigne@ryxeo.com>
#documentation https://redmine.ryxeo.com/projects/ryxeo/wiki/Cr%C3%A9er_un_paquet_avec_OSX
APPNAME="leterrier-fubuki"
APPVERSION="1.1.0"

#On stoppe dès qu'on rencontre un problème
set -e

if [ -d /tmp/build-dmg ]; then
  echo "Erreur: répertoire temporaire de build existant, merci de supprimer /tmp/build-dmg"
  echo "et relancez ce script ensuite ..."
fi
#on fabrique une arborescence "dmg" temporaire
mkdir /tmp/build-dmg

#on cherche ou on se trouve et on change de repertoire le cas echeant
if [ -f ../${APPNAME}.pro ]; then
  cd ..
fi

#qmake
qmake ${APPNAME}.pro -r -spec macx-g++ CONFIG+=release

#compilation
make -w -j4

#on utilise les outils de Qt pour trimbaler le Qt framework et les dependances Qt
macdeployqt ${APPNAME}.app

#copie des petites choses indispensables ...
cp -a ${APPNAME}.app /tmp/build-dmg/
cp -a MacOS/.DS_Store /tmp/build-dmg/
cp -a MacOS/.background /tmp/build-dmg/
cp -a MacOS/Applications /tmp/build-dmg/

#monecoleadistance utilise une commande système qu'il faut embarquer dans le dossier Ressources

#creation du fichier dmg
hdiutil create ${APPNAME}-${APPVERSION}.dmg -srcfolder /tmp/build-dmg -format UDZO -volname ${APPNAME}

#nettoyage
rm -rf /tmp/build-dmg

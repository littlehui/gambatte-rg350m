#!/bin/sh

#BDAT=$(date +"%Y%m%d-%H%M%S")
BDAT=$(date +"%Y%m%d")
echo '#define BUILDDATE "'$BDAT'"' >./gambatte_sdl_legacy/builddate.h

echo "cd libgambatte && scons"
(cd libgambatte && scons -Q target=gcw0) || exit

echo "cd gambatte_sdl_legacy && scons"
(cd gambatte_sdl_legacy && scons -Q target=gcw0)

rm -f gambatte-gcw0-git-$BDAT.opk

mksquashfs ./dist/gcw0/default.gcw0.desktop ./gambatte_sdl_legacy/gambatte_sdl ./dist/gcw0/gambatte.png ./dist/gcw0/manual.txt gambatte-gcw0-git-$BDAT.opk -all-root -no-xattrs -noappend -no-exports

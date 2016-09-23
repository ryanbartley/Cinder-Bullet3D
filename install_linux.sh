#!/bin/sh
# cleanup existing libs
rm -rf lib
# recreate
mkdir -p lib/linux/Debug
# get to build dir
cd bullet3/build3 
# create build make
./premake4_linux64 --no-test --targetdir=../../lib/linux/Debug gmake
cd gmake 
# make configuration
make config=debug64
# cleanup filenames
cd ../../../lib/linux/Debug
for f in *; do
	ext=${f##*.}
	if [ "${ext}" = "a" ]; then
		mv -- "$f" "${f%%_gmake*}.${ext}"
	else 
		rm "$f"
	fi
done
cd ../../..
# recreate
mkdir -p lib/linux/Release
# get to build dir
cd bullet3/build3 
# create build make
./premake4_linux64 --no-test --targetdir=../../lib/linux/Release gmake
cd gmake
# make configuration
make config=release64
# cleanup filenames
cd ../../../lib/linux/Release
for f in *; do
	ext=${f##*.}
	if [ "${ext}" = "a" ]; then
		mv -- "$f" "${f%%_gmake*}.${ext}"
	else 
		rm "$f"
	fi
done
# cleanup build dirs
cd ../../..
rm -rf bullet3/build3/gmake
rm -rf bullet3/Extras/lib

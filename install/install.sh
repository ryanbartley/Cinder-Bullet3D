#!/bin/bash
cd ..

lib_path=""
premake=""
make=""
make_path=""
declare -a config_settings=("debug64" "release64")
declare -a config_paths=("/Debug" "/Release")

lower_case=$(echo "$1" | tr '[:upper:]' '[:lower:]')
echo $lower_case
if [ "${lower_case}" = "mac" ] || [ "${lower_case}" = "macosx" ]; 
then
	lib_path="lib/macosx"
	premake="premake4_osx"
	make="gmake"
	make_path="gmake"
elif [ "${lower_case}" = "linux" ]; 
then
	lib_path="lib/linux"
	premake="premake4_linux64"
	make="gmake"
	make_path="gmake"
elif [ "${lower_case}" = "ios" ];
then
	lib_path="lib/ios"
	premake="premake4_osx --ios"
	make="xcode4"
	make_path="xcode4ios"
	config_settings[0]="Debug"
	config_settings[1]="Release"
else
	echo "Unkown selection: ${1}"
	echo "usage: ./install.sh [platform]"
	echo "accepted platforms are macosx, linux, ios"
	exit 1
fi

for i in 0 1 
do
	config_path=${config_paths[i]}
	config_setting=${config_settings[i]}
	full_lib_path=$lib_path$config_path
	echo "$(pwd)"	
	echo "making directory at, ${full_lib_path}"
	# recreate
	rm -rf $full_lib_path
	mkdir -p $full_lib_path
	# get to build dir
	cd bullet3/build3 
	echo "$(pwd)"
	# create build make	
	./$premake --no-test --targetdir=../../$full_lib_path $make
	cd $make_path
	echo "$(pwd)"	
	if [ "${lower_case}" != "ios" ]; then
		# make configuration
		make config=$config_setting -j 6
		# cleanup filenames
	else
		for proj in *; do
			ext=${proj##*.}
			if [ "${ext}" = "xcodeproj" ]; then
				if [ "${proj%%App*}" != "" ] && [ "${proj%%Test*}" != "" ]; then
					name=${proj%%.*}
				 	xcrun xcodebuild ARCHS=arm64 VALID_ARCHS=arm64 ONLY_ACTIVE_ARCH=NO -project ${name}.${ext} -target ${name} -configuration ${config_setting} -sdk iphoneos
				fi
			fi
		done
	fi
	cd ../../../$full_lib_path
	echo "$(pwd)"
	for f in *; do
		ext=${f##*.}
		if [ "${ext}" = "a" ]; then
			mv -- "$f" "${f%%_$make*}.${ext}"
		else 
			rm "$f"
		fi
	done
	cd ../../..
done

rm -rf bullet3/build3/$make_path
rm -rf bullet3/Extras/lib

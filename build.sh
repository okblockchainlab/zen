#! /bin/sh

BUILD_OS=`uname`
if [ "$BUILD_OS" == "Darwin" ]; then
	./zcutil/build-mac.sh
else
	./zcutil/build.sh
fi

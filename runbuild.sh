#!/usr/bin/env bash

if [ -z "$JAVA_HOME" ]; then
	printf "No JAVA_HOME detected! "
	printf "Setup JAVA_HOME before build: export JAVA_HOME=/path/to/java\\n"
	exit 1
fi

EXT=so
DEPENDS_LIB=
CXX_CMAKE_DEF=

TARGET_OS=`uname -s`
case "$TARGET_OS" in
    Darwin)
        EXT=dylib
				DEPENDS_LIB=`pwd`/depends/`./depends/config.guess`
				CXX_CMAKE_DEF=-DCMAKE_CXX_COMPILER=g++-5
        ;;
    Linux)
        EXT=so
				DEPENDS_LIB=`pwd`/depends/x86_64-unknown-linux-gnu
        ;;
    *)
        echo "Unknown platform!" >&2
        exit 1
esac

./build.sh
[ $? -ne 0 ] && exit 1

PROJECT_NAME=zen
mkdir ok-build
cd ok-build

cmake -DCMAKE_BUILD_TYPE=Release -DENABLE_WALLET=1 -DOKLIBRARY_NAME=$PROJECT_NAME -DDEPENDS_LIB=$DEPENDS_LIB $CXX_CMAKE_DEF ../ok-wallet
[ $? -ne 0 ] && exit 1

make
[ $? -ne 0 ] && exit 1

cp ./lib${PROJECT_NAME}.${EXT} ../
cp ./test_okwallet ../

nm lib${PROJECT_NAME}.${EXT} |grep "[ _]Java_com_okcoin"

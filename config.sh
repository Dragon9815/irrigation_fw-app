#!/bin/sh

do_config() {
    [ ! -d "build/$1" ] && mkdir -p build/$1
    cmake -G"Unix Makefiles" -B"build/$1" -D"CMAKE_BUILD_TYPE=${foo^}" -D"CMAKE_TOOLCHAIN_FILE=cmake/GNU-ARM-Toolchain.cmake" -DCMAKE_EXPORT_COMPILE_COMMANDS=YES 
    ln -s build/"$1"/compile_commands.json .
}

pushd $(dirname $0) >/dev/null

case $1 in
    debug|release)
        do_config $1
        ;;

    *)
        echo "Invalid configuration: '$1'"
        echo "Possible configuration: 'debug', 'release'"
        ;;
esac

popd >/dev/null


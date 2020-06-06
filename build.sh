#!/bin/sh

do_build() {
    [ ! -d "build/$1" ] && . ./config.sh $1
    cd build/$1
    make
}

pushd $(dirname $0) >/dev/null

case $1 in
    debug|release)
        do_build $1
        ;;

    *)
        echo "Invalid configuration: '$1'"
        echo "Possible configuration: 'debug', 'release'"
        ;;
esac

popd >/dev/null


#!/bin/sh

do_debug() {
    arm-none-eabi-gdb -x "gdb/init_$1"
}

pushd $(dirname $0) >/dev/null

case $1 in
    load|attach)
        do_debug "$1" 
        ;;

    *)
        echo "Invalid type: '$1'"
        echo "Possible types: 'load', 'attach'"
        ;;
esac

popd >/dev/null


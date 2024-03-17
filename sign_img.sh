#!/bin/bash
set -eu -o pipefail

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <project_name>"
    exit 1
fi

test -f "$1/out/$1.bin" || {
    echo "$1/out/$1.bin does not exist"
    exit 1
}

python3 lib/mcuboot/scripts/imgtool.py sign \
    --header-size 0x200 \
    --align 4 \
    --slot-size 0xC800 \
    --version 1.0.0 \
    --pad-header \
    $1/out/$1.bin $1/out/signed.bin

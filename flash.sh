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

st-flash write $1/out/$1.bin 0x08000000
st-flash reset

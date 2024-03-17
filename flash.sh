#!/bin/bash
set -eu -o pipefail

if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <bin> <address>"
    exit 1
fi

test -f "$1" || {
    echo "$1 does not exist"
    exit 1
}

st-flash write $1 $2
st-flash reset

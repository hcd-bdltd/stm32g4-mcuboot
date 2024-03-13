#!/bin/bash
set -eu -o pipefail
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <project_name>"
    exit 1
fi

test -f "$1/Makefile" || {
    echo "$1/Makefile does not exist"
    exit 1
}

cd $1
make clean all

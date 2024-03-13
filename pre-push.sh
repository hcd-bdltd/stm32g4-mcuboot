#!/bin/bash
set -eux -o pipefail

./format.sh
./build.sh boot
./build.sh hello1
./build.sh hello2

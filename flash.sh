#!/bin/bash
set -eux -o pipefail

st-flash write $1/out/$1.bin 0x08000000
st-flash reset

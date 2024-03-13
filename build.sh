#!/bin/bash
set -eux -o pipefail

cd $1
make clean all

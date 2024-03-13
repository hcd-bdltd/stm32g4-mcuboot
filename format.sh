#!/bin/bash
set -eux -o pipefail

clang-format -i lib/**.c lib/**.h
clang-format -i boot/**.c boot/**.h
clang-format -i hello1/**.c hello1/**.h
clang-format -i hello2/**.c hello2/**.h

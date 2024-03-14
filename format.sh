#!/bin/bash
set -eux -o pipefail

find boot \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +
find hello1 \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +
find hello2 \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +

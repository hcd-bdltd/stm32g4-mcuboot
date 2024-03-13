#!/bin/bash
set -eux -o pipefail

find lib \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +
find boot \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +
find hello1 \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +
find hello2 \( -name '*.c' -o -name '*.h' \) -exec clang-format -i {} +

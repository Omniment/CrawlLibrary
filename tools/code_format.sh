#!/bin/bash

set -eu

# Move the current directory to the top of git directory
cd $(git rev-parse --show-toplevel)

src_files=$(find src \
                 -type f -regextype posix-egrep \
                 -regex '.*\.([hc](pp)?|cc|ino)$' | sort)

for path in $src_files; do
    echo "Formatting $path..."
    clang-format -i $path
done

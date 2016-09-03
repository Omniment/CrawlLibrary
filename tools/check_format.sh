#!/bin/bash

set -eu

green_echo() {
    echo -e "\033[32m$@\033[0m"
}

red_echo() {
    echo -e "\033[31m$@\033[0m"
}

is_snake_case() {
    local word_pat='([0-9a-z]+|[0-9A-Z]+)'
    local file_pat="${word_pat}(_${word_pat})*"
    echo "$1" \
    | sed 's|^\./||' \
    | sed 's/\..*$//' \
    | grep -P "^$file_pat(/$file_pat)*\$" 2>/dev/null 1>&2
}

# Move the current directory to the top of git directory
cd $(git rev-parse --show-toplevel)

flag=0 # STATUS CODE: 0 if all checks have been passed.

src_files=$(find src \
                 -type f -regextype posix-egrep \
                 -regex '.*\.([hc](pp)?|cc|ino)$' | sort)

echo 'Checking filenames...'
for path in $src_files
do
    if is_snake_case "$path"; then
        green_echo "[Passed] Filename $path is well-formed"
    else
        red_echo "[Failed] Filename $path is ill-formed"
        flag=127
    fi
done
echo

echo 'Checking code format...'
for path in $src_files
do
    clang-format $path > $path.tmp
    if [ "$(diff "$path" "$path.tmp")" == "" ]; then
        green_echo "[Passed] File $path is formatted"
    else
        red_echo "[Failed] File $path is unformatted"
        red_echo "  Please run ./tools/code_format.sh on your local repository!"
        flag=127
    fi
    rm -f $path.tmp
done

exit $flag

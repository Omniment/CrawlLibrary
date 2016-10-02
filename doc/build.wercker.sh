#!/bin/bash

##
## build.wercker.sh --- a documentation build tool on CI service
##
## Usage: ./doc/build.wercker.sh REPOSITORY_URL
##

set -eu

red_echo() {
    echo -e "\033[31m$@\033[0m"
}

green_echo() {
    echo -e "\033[32m$@\033[0m"
}

cyan_echo() {
    echo -e "\033[36m$@\033[0m"
}

## Build documentation by doxygen
build() {
    doxyfile=$1
    version=$2
    cat "$doxyfile" \
    | sed "s/__VERSION__/$version/g" \
    | doxygen -
}

github_repo=$1
tags=$(git tag) # all tags at the current branch

if [ "$tags" == "" ]
then
    cyan_echo "[Skip] No tags are found at the current branch."
    cyan_echo "       Documentation won't be updated."
else
    git config --global user.email "dummy@omniment.co.jp"
    git config --global user.name "Omniment"
    git clone -b gh-pages "$github_repo" gh-pages

    updated_flag=0 # 1 if new document is added.
    for version in $tags
    do
        if [ -d "./gh-pages/$version" ]
        then
            cyan_echo "[Skip] Documentation '$version' already exists."
        else
            green_echo "Checkout '$version'..."
            git checkout "refs/tags/$version"

            green_echo "Building document '$version' for users..."
            build "doc/user.doxyfile" "$version"

            green_echo "Building document '$version' for developers..."
            build "doc/develop.doxyfile" "$version"

            green_echo "Adding documentation to gh-pages..."
            cd gh-pages/

            if [ -d "./develop" ]; then git rm -rf "./develop"; fi
            if [ -d "./latest"  ]; then git rm -rf "./latest"; fi

            mv "../doc/develop/html" "./develop"
            mv "../doc/user/html" "./$version"
            cp -r "./$version" "./latest"

            git add "./develop" "./$version" "./latest"
            git commit -m "Add documentation $version"

            cd ..

            updated_flag=1
        fi
    done

    ## Push documents if updated.
    if [ $updated_flag -eq 1 ]; then
        green_echo "Publishing documentation..."
        cd gh-pages/
        git push origin gh-pages:gh-pages
    fi
fi

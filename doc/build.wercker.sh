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

if [ "$github_repo" == "" ]
then
    cyan_echo "The URL of a GitHub repository is not given. Nothing to do."
    exit 0
fi

## Update remote repo
git remote remove origin
git remote add origin "$github_repo"

## Clone 'gh-pages' branch
git config --global user.email "dummy@omniment.co.jp"
git config --global user.name "Omniment"
git clone -b gh-pages "$github_repo" gh-pages

##
## Build documentation for developers
##
ln -sf /usr/share/zoneinfo/Asia/Tokyo /etc/localtime # Change the current time zone

green_echo "Building documentation for developers..."
git checkout master
git pull origin master
build "doc/develop.doxyfile" "$(date '+%Y-%m-%d_%H:%M:%S')"

green_echo "Adding documentation to gh-pages..."
cd gh-pages/

if [ -d "./develop" ]; then git rm -rf "./develop"; fi

mv "../doc/develop/html" "./develop"

git add "./develop"
git commit -m "Add documentation 'develop'"

cd ..

##
## Build documentation for users
##
git fetch --tags
tags=$(git tag) # all tags at the current branch

if [ "$tags" == "" ]
then
    cyan_echo "[Skip] No tags are found at the current branch."
    cyan_echo "       Documentation won't be updated."
else
    for version in $tags
    do
        if [ -d "./gh-pages/$version" ]
        then
            cyan_echo "[Skip] Documentation '$version' already exists."
        else
            green_echo "Checkout '$version'..."
            git checkout "refs/tags/$version"

            green_echo "Building documentation '$version' for users..."
            build "doc/user.doxyfile" "$version"

            green_echo "Adding documentation to gh-pages..."
            cd gh-pages/

            if [ -e "./latest" ]; then git rm -rf "./latest"; fi

            mv "../doc/user/html" "./$version"
            ln -sf "./$version" "./latest"

            git add "./$version" "./latest"
            git commit -m "Add documentation '$version'"

            cd ..
        fi
    done
fi

## Upload built documents.
green_echo "Publishing documentation..."
cd gh-pages/
git push origin gh-pages:gh-pages || :

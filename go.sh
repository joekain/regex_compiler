#! /bin/bash -e

# This script should do everything.  That is build, run tests, etc.
# It will become better organized over time.

cd $(dirname "$0")

mkdir -p build
cd build

if [ ! -f Makefile ]; then
    cmake ..
fi

make
ctest
./run-clang-tidy.py

# TODO: Check formatting

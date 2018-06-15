#!/bin/bash -e

#This script should do everything.That is build, run tests, etc.
#It will become better organized over time.

cd $(dirname "$0")

mkdir -p build
cd build

if [ ! -f Makefile ]; then
    cmake ..
fi

make
ctest --output-on-failure
./run-clang-tidy.py -quiet

for file in $(ls /tmp/*.dot); do
    dot -T pdf -O $file
done

#TODO : Check formatting

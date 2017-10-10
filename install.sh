#!/bin/sh

# This script is a part of librealtime.
# THis script mv librealtime direcotry to /usr/local/include.

echo Start Installation of librealtime

cp -r librealtime /usr/local/include

echo Make the documentation

mkdir doc
doxygen Doxyfile

echo Finish Installation of librealtime


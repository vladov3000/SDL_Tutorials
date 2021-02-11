#!/usr/bin/env bash
# given url as argument, downloads and extracts next exercise

filename=$(basename -- "$1")
prefix="${filename%.*}"

wget $1
unzip $prefix.zip
rm $prefix.zip
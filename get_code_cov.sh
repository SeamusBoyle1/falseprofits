#!/bin/bash

GCOV="gcov-4.9"

for filename in `find . -not -path "./boost*" -not -path "./Qt/*" | egrep '\.cpp'`;
do
    out_dirname=$(echo $filename | cut -d"/" -f2-)
    $GCOV -n -o $out_dirname $filename > /dev/null;
done

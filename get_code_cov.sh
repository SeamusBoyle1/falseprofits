#!/bin/bash

# http://stackoverflow.com/questions/59895/can-a-bash-script-tell-what-directory-its-stored-in
SCRIPT_DIR_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

GCOV="gcov-4.9"

# Assumes build dir and source dir are siblings
SOURCE_DIR="../$(echo $(basename $SCRIPT_DIR_PATH))"

for filename in `find $SOURCE_DIR -not -path "$SOURCE_DIR/boost*" -not -path "$SOURCE_DIR/Qt/*" | egrep '\.cpp'`;
do
    out_dirname=$(echo $filename | cut -d"/" -f3-)
    $GCOV -n -o $out_dirname $filename > /dev/null;
done

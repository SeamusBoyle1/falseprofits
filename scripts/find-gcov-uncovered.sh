#!/bin/bash -e

# Prints *.cpp and *.h files unknown to lcov
#
# The files need to be added to a test for coverage %
# to be accurate

if [ ! -f VERSION ]; then
    echo "Need to run this script from root directory"
    exit 1
fi

if [ -d .git ]; then
    ACTUAL_FILE_LIST=$(git ls-files '*.cpp' '*.h' | sort)
else
    ACTUAL_FILE_LIST=$(find . -name '*.cpp' ! -name 'moc_*.cpp' ! -name 'qrc_*.cpp' -o -name '*.h' ! -name 'moc_*.h' ! -name 'ui_*.h' | cut -c3- | sort)
fi

ACTUAL_FILE_LIST=$(echo "$ACTUAL_FILE_LIST" | grep -v '^tests/' | grep -v '/tests/' | grep -v '/examples/')

TRACEFILE="$1"

FILES_WITH_TEST=$(lcov -l "$TRACEFILE" --list-full-path -q | cut -d\| -f1 | sed 's/[ \t]*$//' | grep -oP "^$PWD/\K.*" | sort)

diff <(echo "$ACTUAL_FILE_LIST") <(echo "$FILES_WITH_TEST") | grep -e '^<' | cut -c3-


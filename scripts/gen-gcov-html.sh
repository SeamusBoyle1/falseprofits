#!/bin/bash -e

if [ ! -f VERSION ]; then
    echo "Need to run this script from root directory"
    exit 1
fi

if [ -d .git ]; then
    REVISION="$(git rev-parse HEAD)"
else
    REVISION="$(cat REVISION)"
fi

lcov --capture --initial --directory . --output-file coverage-gcov-baseline.info --no-external

lcov --capture --directory . --output-file coverage-gcov-test.info --no-external

lcov --add-tracefile coverage-gcov-baseline.info \
    --add-tracefile coverage-gcov-test.info \
    --output-file coverage-gcov-total.info

lcov --output-file coverage-gcov-total.info \
    --remove coverage-gcov-total.info '*.moc*' '.*rcc*' '*3rdparty*' '*/tests/*' '*/examples/*' '*/moc_*.cpp' '*/ui_*.h' '*/qrc_*.cpp'

./scripts/find-gcov-uncovered.sh coverage-gcov-total.info > coverage-gcov-unknown-coverage.txt

rm -rf doc/coverage
genhtml coverage-gcov-total.info --legend --title "commit $REVISION" --output-directory doc/coverage

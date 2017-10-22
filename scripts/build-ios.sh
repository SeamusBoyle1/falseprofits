#!/bin/bash -e

if [ -z "$1" ]
then
    echo "No build configuration specified"
    exit
fi

if [ -z "$2" ]
then
    echo 'Need to specify "device" or "simulator" as second arg'
    exit
fi

OUTPUT_FOR="$2"
OUTPUT_FOR_QMAKE_ARGS=""
DTYPE=""

if [ "$OUTPUT_FOR" == "device" ]
then
    OUTPUT_FOR_QMAKE_ARGS="CONFIG += iphoneos device"
    DTYPE="iphoneos"
elif [ "$OUTPUT_FOR" == "simulator" ]
then
    OUTPUT_FOR_QMAKE_ARGS="CONFIG += iphonesimulator simulator"
    DTYPE="iphonesimulator"
else
    echo "Unknown output target: $OUTPUT_FOR"
    exit
fi

source "$1"

if [ -z "$BOOST_ROOT" ]; then
    echo "BOOST_ROOT environment variable needs to be set"
    exit 1
fi

if [ ! -z "$QTDIR" ]; then
    export QTDIR
    export PATH=$QTDIR/bin:$PATH
fi

export BOOST_ROOT

# http://stackoverflow.com/questions/59895/can-a-bash-script-tell-what-directory-its-stored-in
SCRIPT_DIR_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

SOURCE_DIR="$SCRIPT_DIR_PATH/.."
OUT_DIR=$PWD

pushd "$SOURCE_DIR" && \
GIT_DESCRIBE=$(git describe)
popd
echo "Building $GIT_DESCRIBE"

# For signing stuff refer to comments at https://bugreports.qt.io/browse/QTBUG-38782
qmake "$SOURCE_DIR" -spec macx-ios-clang \
    "$OUTPUT_FOR_QMAKE_ARGS" "$USE_QMAKE_XCODE_CODE_SIGN_IDENTITY" "$USE_QMAKE_DEVELOPMENT_TEAM" "$USE_QMAKE_PROVISIONING_PROFILE" \
    "CONFIG += release" "BOOST_ROOT=$BOOST_ROOT"
make qmake_all
pushd src && make ; popd
#make check

APP_QMLDIR="$SOURCE_DIR/src/FalseProfitsMobile"

rm -rf "$OUT_DIR/${DTYPE}-build"
mkdir -p "$OUT_DIR/${DTYPE}-build/Payload"
cp -pR "$OUT_DIR/src/FalseProfitsMobile/Release-$DTYPE/FalseProfitsMobile.app" "$OUT_DIR/${DTYPE}-build/Payload/"
pushd "$OUT_DIR/${DTYPE}-build" && \
/usr/bin/zip --symlinks --verbose --recurse-paths "$OUT_DIR/FalseProfitsMobile-$GIT_DESCRIBE-$DTYPE.ipa" .
popd
echo "Output: $OUT_DIR/FalseProfitsMobile-$GIT_DESCRIBE-$DTYPE.ipa"

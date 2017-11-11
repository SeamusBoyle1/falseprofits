#!/bin/bash -e

if [ -z "$1" ]
then
    echo "No build configuration specified"
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

rm -rf src/FalseProfitsMobile/FalseProfitsMobile.app/

qmake -r "$SOURCE_DIR"
make
make check

APP_QMLDIR="$SOURCE_DIR/src/FalseProfitsMobile"

# Workaround for macdeployqt missing Controls 1, QTBUG-59054
APP_BUNDLE="$PWD"/src/FalseProfitsMobile/FalseProfitsMobile.app
mkdir -p "$APP_BUNDLE"/Contents/Resources/qml/QtQuick
cp -r "$QTDIR"/qml/QtQuick/Controls "$APP_BUNDLE"/Contents/Resources/qml/QtQuick/
rm -f "$APP_BUNDLE"/Contents/Resources/qml/QtQuick/Controls/libqtquickcontrolsplugin_debug.dylib

macdeployqt \
    src/FalseProfitsMobile/FalseProfitsMobile.app \
    -qmldir="$APP_QMLDIR" \
    -dmg

pushd src/FalseProfitsMobile && \
ln FalseProfitsMobile.dmg "$OUT_DIR/FalseProfitsMobile-$GIT_DESCRIBE-mac-x86_64.dmg"
popd

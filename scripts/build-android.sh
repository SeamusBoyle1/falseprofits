#!/bin/bash -e

if [ -z "$1" ]
then
    echo "No build configuration specified"
    exit
fi

source "$1"

if [ -z "$KEY_FILE" ]; then
    echo "KEY_FILE environment variable needs to be set"
    exit 1
fi

if [ -z "$CERT_ALIAS" ]; then
    echo "CERT_ALIAS environment variable needs to be set"
    exit 1
fi

if [ -z "$KEYSTORE_PASS" ]; then
    echo "KEYSTORE_PASS environment variable needs to be set"
    exit 1
fi

if [ -z "$BOOST_ROOT" ]; then
    echo "BOOST_ROOT environment variable needs to be set"
    exit 1
fi

if [ -z "$ANDROID_NDK_ROOT" ]; then
    echo "ANDROID_NDK_ROOT environment variable needs to be set"
    exit 1
fi

if [ -z "$ANDROID_SDK_ROOT" ]; then
    echo "ANDROID_SDK_ROOT environment variable needs to be set"
    exit 1
fi

if [ ! -z "$JAVA_HOME" ]; then
    export JAVA_HOME
    export PATH=$JAVA_HOME/bin:$PATH
fi

if [ ! -z "$QTDIR" ]; then
    export QTDIR
    export PATH=$QTDIR/bin:$PATH
fi

export BOOST_ROOT

export ANDROID_NDK_ROOT
export ANDROID_SDK_ROOT

# http://stackoverflow.com/questions/59895/can-a-bash-script-tell-what-directory-its-stored-in
SCRIPT_DIR_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

SOURCE_DIR="$SCRIPT_DIR_PATH/.."
OUT_DIR=$PWD

pushd "$SOURCE_DIR" && \
GIT_DESCRIBE=$(git describe)
popd

qmake -r "$SOURCE_DIR"
make
#make check

rm -rf "$OUT_DIR/android-build/assets"
rm -rf "$OUT_DIR/android-build/libs"
# don't want to make install in tests folder, so cd into src
( cd src && make "INSTALL_ROOT=$OUT_DIR/android-build" install )
# remove static libs
rm "$OUT_DIR"/android-build/usr/lib/*.a
( cd "$OUT_DIR/android-build" && rmdir -p usr/lib )

OPENSSL_ROOT="$OUT_DIR/openssl-1.0.2l"
if [[ ! -f "$OPENSSL_ROOT/libcrypto.so" ]] || [[ ! -f "$OPENSSL_ROOT/libssl.so" ]]; then
    rm -rf "$OPENSSL_ROOT"
    if [ ! -f openssl-1.0.2l.tar.gz ]; then
        wget https://www.openssl.org/source/openssl-1.0.2l.tar.gz
    fi
    tar -xf openssl-1.0.2l.tar.gz
    (
        export _ANDROID_NDK="android-ndk-r10"
        export _ANDROID_EABI="arm-linux-androideabi-4.9"
        export _ANDROID_ARCH=arch-arm
        export _ANDROID_API="android-16"

        cd openssl-1.0.2l
        . "$SOURCE_DIR/scripts/openssl/Setenv-android.sh"
        ./Configure shared android
        make CALC_VERSIONS="SHLIB_COMPAT=; SHLIB_SOVER=" build_libs
        cp -p libcrypto.so "$OUT_DIR/android-build/libs/armeabi-v7a/"
        cp -p libssl.so "$OUT_DIR/android-build/libs/armeabi-v7a/"
    )
fi

rm -rf "$OUT_DIR/android-build/build/outputs/apk/android-build-*.apk"

androiddeployqt \
    --input "$OUT_DIR/src/FalseProfitsMobile/android-libFalseProfitsMobile.so-deployment-settings.json" \
    --output "$OUT_DIR/android-build" \
    --deployment bundled \
    --android-platform android-26 \
    --gradle \
    --sign "$KEY_FILE" "$CERT_ALIAS" \
    --storepass "$KEYSTORE_PASS"

pushd "$OUT_DIR/android-build/build/outputs/apk" && \
ln android-build-release-signed.apk "$OUT_DIR/FalseProfitsMobile-$GIT_DESCRIBE-android-armv7.apk"
popd

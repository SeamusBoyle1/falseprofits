#-------------------------------------------------
#
# Project created by QtCreator 2017-09-21T05:51:11
#
#-------------------------------------------------

include($$PWD/../../testcase.pri)

QT       += testlib

QT       -= gui

TARGET = tst_jsonresponsemetatypewrapperstest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        tst_jsonresponsemetatypewrapperstest.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += $$PWD/../../../../src/FalseProfitsCore \
    $$PWD/../../../../src

HEADERS += \
    $$PWD/../../../../src/FalseProfitsCore/fpdeclarativetypes.h

# build deps
include($$PWD/../../../../src/3rdparty/boost_h.prf)

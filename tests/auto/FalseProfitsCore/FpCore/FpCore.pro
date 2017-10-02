#-------------------------------------------------
#
# Project created by QtCreator 2017-09-12T15:07:16
#
#-------------------------------------------------

include($$PWD/../../testcase.pri)

QT       += network testlib

QT       -= gui

TARGET = tst_fpcoretest
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
        tst_fpcoretest.cpp 

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += $$PWD/../../../../src/FalseProfitsCore \
    $$PWD/../../../../src

SOURCES += \
    $$PWD/../../../../src/FalseProfitsCore/fpcore.cpp

HEADERS += \
    $$PWD/../../../../src/FalseProfitsCore/fpcore.h

# build deps
include($$PWD/../../../../src/3rdparty/boost_h.prf)

HEADERS += \
    $$PWD/../../../../src/FalseProfitsCore/fpcore_interfaces.h \
    $$PWD/../../../../src/FalseProfitsCore/fpdeclarativetypes.h \
    $$PWD/../../../../src/FalseProfitsCore/responsetypes.h

HEADERS += \
    $$PWD/../../../../src/InvestorAPIClient/iinvestorapiclient.h \

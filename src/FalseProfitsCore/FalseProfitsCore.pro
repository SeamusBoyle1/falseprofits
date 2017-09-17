#-------------------------------------------------
#
# Project created by QtCreator 2017-09-10T10:32:46
#
#-------------------------------------------------

QT       -= gui

TARGET = FalseProfitsCore
TEMPLATE = lib
CONFIG += c++11
CONFIG += staticlib

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
    fpsettings.cpp \
    fpcore.cpp

HEADERS += \
    responsetypes.h \
    fpsettings.h \
    fpcore.h \
    fpdeclarativetypes.h \
    fpcore_interfaces.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

## Boost
include($$PWD/../3rdparty/boost_h.prf)

## InvestorAPIClient
QT += network
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../InvestorAPIClient/release/ -lInvestorAPIClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../InvestorAPIClient/debug/ -lInvestorAPIClient
else:unix: LIBS += -L$$OUT_PWD/../InvestorAPIClient/ -lInvestorAPIClient

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../InvestorAPIClient/release/libInvestorAPIClient.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../InvestorAPIClient/debug/libInvestorAPIClient.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../InvestorAPIClient/release/InvestorAPIClient.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../InvestorAPIClient/debug/InvestorAPIClient.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../InvestorAPIClient/libInvestorAPIClient.a

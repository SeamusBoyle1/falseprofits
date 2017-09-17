QT += quick
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/..
DEPENDPATH += $$PWD/..

## Boost
include($$PWD/../3rdparty/boost_h.prf)

## FalseProfitsCore
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../FalseProfitsCore/release/ -lFalseProfitsCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../FalseProfitsCore/debug/ -lFalseProfitsCore
else:unix: LIBS += -L$$OUT_PWD/../FalseProfitsCore/ -lFalseProfitsCore

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FalseProfitsCore/release/libFalseProfitsCore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FalseProfitsCore/debug/libFalseProfitsCore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FalseProfitsCore/release/FalseProfitsCore.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../FalseProfitsCore/debug/FalseProfitsCore.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../FalseProfitsCore/libFalseProfitsCore.a

## InvestorAPIClient
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../InvestorAPIClient/release/ -lInvestorAPIClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../InvestorAPIClient/debug/ -lInvestorAPIClient
else:unix: LIBS += -L$$OUT_PWD/../InvestorAPIClient/ -lInvestorAPIClient

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../InvestorAPIClient/release/libInvestorAPIClient.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../InvestorAPIClient/debug/libInvestorAPIClient.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../InvestorAPIClient/release/InvestorAPIClient.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../InvestorAPIClient/debug/InvestorAPIClient.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../InvestorAPIClient/libInvestorAPIClient.a

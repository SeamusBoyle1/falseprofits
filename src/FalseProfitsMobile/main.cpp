// Copyright 2017

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <FalseProfitsCore/fpcore.h>
#include <FalseProfitsCore/responsetypes.h>
#include <FalseProfitsCore/fpsettings.h>
#include <InvestorAPIClient/iinvestorapiclient.h>

#include <QDateTime>
#include <QQmlContext>
#include <QSettings>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName(QStringLiteral("QtHelloWorld"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("student.rmit.edu.au"));
    QCoreApplication::setApplicationName(QStringLiteral("FalseProfits"));

    QGuiApplication app(argc, argv);

    qmlRegisterType<NewUserResponse>("FpResponses", 1, 0, "NewUserResponse");
    qmlRegisterType<AuthenticateResponse>("FpResponses", 1, 0, "AuthenticateResponse");
    qmlRegisterType<DeleteUserResponse>("FpResponses", 1, 0, "DeleteUserResponse");
    qmlRegisterType<GetUserProfileResponse>("FpResponses", 1, 0, "GetUserProfileResponse");

    qmlRegisterUncreatableMetaObject(Fpx::staticMetaObject, "com.example.fpx", 1, 0, "Fpx",
                                     "Access to enums & flags only");

#ifdef Q_OS_WIN
    // Don't use Windows registry
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    auto client = bsmi::InvestorAPIClientFactory::create();
    FpSettings fpCoreSettings;
    FpCore fpCore(client, &fpCoreSettings);
    client->setParent(&fpCore);
    FalseProfitsDeclarativeTypes fpTypes;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("fpCore", &fpCore);
    engine.rootContext()->setContextProperty("fpType", &fpTypes);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    fpCore.loadLaunchSettings();

    return app.exec();
}

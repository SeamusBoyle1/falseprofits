// Copyright 2017

#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <FalseProfitsCore/finishnotifier.h>
#include <FalseProfitsCore/fpaccountslistmodel.h>
#include <FalseProfitsCore/fpcore.h>
#include <FalseProfitsCore/fpdeclarativetypes.h>
#include <FalseProfitsCore/responsetypes.h>
#include <FalseProfitsCore/fpsettings.h>
#include <FalseProfitsCore/fpsymbolsearchresultsitemmodel.h>
#include <FalseProfitsCore/fpsymbolsearchwrapper.h>
#include <FalseProfitsCore/fptradingaccounts.h>
#include <InvestorAPIClient/iinvestorapiclient.h>

#include <QDateTime>
#include <QFileSelector>
#include <QQmlContext>
#include <QQmlFileSelector>
#include <QSettings>

constexpr bool isHostMobile()
{
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    return true;
#else
    return false;
#endif
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName(QStringLiteral("QtHelloWorld"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("student.rmit.edu.au"));
    QCoreApplication::setApplicationName(QStringLiteral("FalseProfits"));

    QGuiApplication app(argc, argv);

    qmlRegisterSingletonType(QUrl("qrc:/FpStyle.qml"), "com.example.fpx", 1, 0, "FpStyle");

    qmlRegisterUncreatableType<FpCore>("FpCore", 1, 0, "FpCore", "For setter injection only");

    qmlRegisterType<NewUserResponse>("FpResponses", 1, 0, "NewUserResponse");
    qmlRegisterType<AuthenticateResponse>("FpResponses", 1, 0, "AuthenticateResponse");
    qmlRegisterType<DeleteUserResponse>("FpResponses", 1, 0, "DeleteUserResponse");
    qmlRegisterType<GetUserProfileResponse>("FpResponses", 1, 0, "GetUserProfileResponse");
    qmlRegisterType<GetQuotesResponse>("FpResponses", 1, 0, "GetQuotesResponse");
    qmlRegisterType<SymbolSearchResponse>("FpResponses", 1, 0, "SymbolSearchResponse");
    qmlRegisterType<GetShareDetailsResponse>("FpResponses", 1, 0, "GetShareDetailsResponse");
    qmlRegisterType<SendOrderResponse>("FpResponses", 1, 0, "SendOrderResponse");

    qmlRegisterType<FinishNotifier>("com.example.fpx", 1, 0, "FinishNotifier");
    qmlRegisterType<FpSymbolSearchWrapper>("com.example.fpx", 1, 0, "FpSymbolSearchWrapper");
    qmlRegisterType<FpSymbolSearchResultsItemModel>("com.example.fpx", 1, 0,
                                                    "FpSymbolSearchResultsItemModel");
    qmlRegisterType<FpAccountsListModel>("com.example.fpx", 1, 0, "FpAccountsListModel");
    qmlRegisterType<FpTradingAccounts>("com.example.fpx", 1, 0, "FpTradingAccounts");

    // Q_ENUM inside a class using Q_GADGET
    qmlRegisterUncreatableType<OrderParams>("com.example.fpx", 1, 0, "OrderParams",
                                            "Access to enums only");

    // Q_ENUM_NS inside a namespace using Q_NAMESPACE
    qmlRegisterUncreatableMetaObject(Fpx::staticMetaObject, "com.example.fpx", 1, 0, "Fpx",
                                     "Access to enums & flags only");

#ifdef Q_OS_WIN
    // Don't use Windows registry
    QSettings::setDefaultFormat(QSettings::IniFormat);
#endif

    auto client = bsmi::InvestorAPIClientFactory::create(
        QStringLiteral("https://investor-api.herokuapp.com"));
    FpSettings fpCoreSettings;
    FpCore fpCore(client, &fpCoreSettings);
    client->setParent(&fpCore);
    FalseProfitsDeclarativeTypes fpTypes;

    QQmlApplicationEngine engine;
    if (!(isHostMobile() || app.arguments().contains(QLatin1String("-touch")))) {
        QQmlFileSelector::get(&engine)->setExtraSelectors({ QLatin1String("desktop") });
    }
    engine.rootContext()->setContextProperty("fpCore", &fpCore);
    engine.rootContext()->setContextProperty("fpType", &fpTypes);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    fpCore.loadLaunchSettings();

    return app.exec();
}

// Copyright 2017

#include <QApplication>
#include <QQmlApplicationEngine>

#include <FalseProfitsCore/brokeragecostcalculator.h>
#include <FalseProfitsCore/finishnotifier.h>
#include <FalseProfitsCore/fpaccountslistmodel.h>
#include <FalseProfitsCore/fpcore.h>
#include <FalseProfitsCore/fpdeclarativetypes.h>
#include <FalseProfitsCore/responsetypes.h>
#include <FalseProfitsCore/fppositionslistmodel.h>
#include <FalseProfitsCore/fpportfoliowrapper.h>
#include <FalseProfitsCore/fpsettings.h>
#include <FalseProfitsCore/fpsymbolsearchresultsitemmodel.h>
#include <FalseProfitsCore/fpsymbolsearchwrapper.h>
#include <FalseProfitsCore/fptradingaccounts.h>
#include <FalseProfitsCore/fptransactionslistmodel.h>
#include <FalseProfitsCore/fptransactionswrapper.h>
#include <FalseProfitsCore/fpwatchlistlistmodel.h>
#include <FalseProfitsCore/fpwatchlists.h>
#include <FalseProfitsCore/fpwatchlistslistmodel.h>
#include <FalseProfitsCore/fpwatchlistwrapper.h>
#include <FalseProfitsMobile/fpchartdatawrapper.h>
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

    QApplication app(argc, argv);

    qmlRegisterSingletonType(QUrl("qrc:/FpStyle.qml"), "com.example.fpx", 1, 0, "FpStyle");

    qmlRegisterUncreatableType<FpCore>("FpCore", 1, 0, "FpCore", "For setter injection only");

    qmlRegisterType<NewUserResponse>("FpResponses", 1, 0, "NewUserResponse");
    qmlRegisterType<AuthenticateResponse>("FpResponses", 1, 0, "AuthenticateResponse");
    qmlRegisterType<DeleteUserResponse>("FpResponses", 1, 0, "DeleteUserResponse");
    qmlRegisterType<GetUserProfileResponse>("FpResponses", 1, 0, "GetUserProfileResponse");
    qmlRegisterType<GetPositionsResponse>("FpResponses", 1, 0, "GetPositionsResponse");
    qmlRegisterType<GetTransactionsResponse>("FpResponses", 1, 0, "GetTransactionsResponse");
    qmlRegisterType<GetQuotesResponse>("FpResponses", 1, 0, "GetQuotesResponse");
    qmlRegisterType<GetCandlesResponse>("FpResponses", 1, 0, "GetCandlesResponse");
    qmlRegisterType<SymbolSearchResponse>("FpResponses", 1, 0, "SymbolSearchResponse");
    qmlRegisterType<GetShareDetailsResponse>("FpResponses", 1, 0, "GetShareDetailsResponse");
    qmlRegisterType<SendOrderResponse>("FpResponses", 1, 0, "SendOrderResponse");

    qmlRegisterType<BrokerageCostCalculator>("com.example.fpx", 1, 0, "BrokerageCostCalculator");
    qmlRegisterType<FinishNotifier>("com.example.fpx", 1, 0, "FinishNotifier");
    qmlRegisterType<FpSymbolSearchWrapper>("com.example.fpx", 1, 0, "FpSymbolSearchWrapper");
    qmlRegisterType<FpSymbolSearchResultsItemModel>("com.example.fpx", 1, 0,
                                                    "FpSymbolSearchResultsItemModel");
    qmlRegisterType<FpAccountsListModel>("com.example.fpx", 1, 0, "FpAccountsListModel");
    qmlRegisterType<FpPositionsListModel>("com.example.fpx", 1, 0, "FpPositionsListModel");
    qmlRegisterType<FpPortfolioWrapper>("com.example.fpx", 1, 0, "FpPortfolioWrapper");
    qmlRegisterType<FpChartDataWrapper>("com.example.fpx", 1, 0, "FpChartDataWrapper");
    qmlRegisterType<FpTradingAccounts>("com.example.fpx", 1, 0, "FpTradingAccounts");
    qmlRegisterType<FpTransactionsListModel>("com.example.fpx", 1, 0, "FpTransactionsListModel");
    qmlRegisterType<FpTransactionsWrapper>("com.example.fpx", 1, 0, "FpTransactionsWrapper");
    qmlRegisterType<FpWatchlistListModel>("com.example.fpx", 1, 0, "FpWatchlistListModel");
    qmlRegisterType<FpWatchlists>("com.example.fpx", 1, 0, "FpWatchlists");
    qmlRegisterType<FpWatchlistsListModel>("com.example.fpx", 1, 0, "FpWatchlistsListModel");
    qmlRegisterType<FpWatchlistWrapper>("com.example.fpx", 1, 0, "FpWatchlistWrapper");

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

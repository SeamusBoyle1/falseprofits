// Copyright 2017

#include <QApplication>
#include <QQmlApplicationEngine>

#include <FalseProfitsCore/brokeragecostcalculator.h>
#include <FalseProfitsCore/finishnotifier.h>
#include <FalseProfitsCore/fpaccountslistmodel.h>
#include <FalseProfitsCore/fpcore.h>
#include <FalseProfitsCore/fpleaderboardlistmodel.h>
#include <FalseProfitsCore/fpleaderboardwrapper.h>
#include <FalseProfitsCore/fplocale.h>
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
#include <FalseProfitsCore/utilityfunctionswrapper.h>
#include <FalseProfitsMobile/fpchartdatawrapper.h>
#include <InvestorAPIClient/iinvestorapiclient.h>

#include <QDateTime>
#include <QDebug>
#include <QFileSelector>
#include <QFontDatabase>
#include <QQmlContext>
#include <QQmlFileSelector>
#include <QSettings>

#include <array>

constexpr bool isHostMobile()
{
#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS)
    return true;
#else
    return false;
#endif
}

constexpr bool isHostAndroid()
{
#if defined(Q_OS_ANDROID)
    return true;
#else
    return false;
#endif
}

void loadRobotoFont()
{
    std::array<QString, 18> fonts{ {
        // Workaround for Normal weighted font must be inserted
        // before heavier fonts. Keep fonts group ordering the
        // same as QFont::Weight

        "Roboto-Thin.ttf", //
        "Roboto-ThinItalic.ttf", //
        "Roboto-Light.ttf", //
        "Roboto-LightItalic.ttf", //
        "Roboto-Regular.ttf", //
        "Roboto-Italic.ttf", //
        "Roboto-Medium.ttf", //
        "Roboto-MediumItalic.ttf", //
        "Roboto-Bold.ttf", //
        "Roboto-BoldItalic.ttf", //
        "Roboto-Black.ttf", //
        "Roboto-BlackItalic.ttf", //

        "RobotoCondensed-Light.ttf", //
        "RobotoCondensed-LightItalic.ttf", //
        "RobotoCondensed-Regular.ttf", //
        "RobotoCondensed-Italic.ttf", //
        "RobotoCondensed-Bold.ttf", //
        "RobotoCondensed-BoldItalic.ttf", //
    } };

    QString prefix{ ":/fonts/" };
    QFontDatabase fontDatabase;
    for (auto const &e : fonts) {
        QString fileName{ prefix + e };
        if (fontDatabase.addApplicationFont(fileName) == -1) {
            qWarning() << "Failed to load font:" << fileName;
        }
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QCoreApplication::setOrganizationName(QStringLiteral("QtHelloWorld"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("student.rmit.edu.au"));
    QCoreApplication::setApplicationName(QStringLiteral("FalseProfits"));
    QGuiApplication::setApplicationDisplayName(QObject::tr("False Profits"));

    QApplication app(argc, argv);

    if (!isHostAndroid()) {
        loadRobotoFont();
    }

    qmlRegisterSingletonType(QUrl("qrc:/FpStyle.qml"), "com.example.fpx", 1, 0, "FpStyle");
    qmlRegisterSingletonType(QUrl("qrc:/ExtraMaterial.qml"), "io.material.xtra", 1, 0,
                             "ExtraMaterial");

    qmlRegisterUncreatableType<FpCore>("FpCore", 1, 0, "FpCore", "For setter injection only");

    qmlRegisterType<NewUserResponse>("FpResponses", 1, 0, "NewUserResponse");
    qmlRegisterType<AuthenticateResponse>("FpResponses", 1, 0, "AuthenticateResponse");
    qmlRegisterType<DeleteUserResponse>("FpResponses", 1, 0, "DeleteUserResponse");
    qmlRegisterType<GetUserProfileResponse>("FpResponses", 1, 0, "GetUserProfileResponse");
    qmlRegisterType<EditUserProfileResponse>("FpResponses", 1, 0, "EditUserProfileResponse");
    qmlRegisterType<GetPositionsResponse>("FpResponses", 1, 0, "GetPositionsResponse");
    qmlRegisterType<ResetAccountResponse>("FpResponses", 1, 0, "ResetAccountResponse");
    qmlRegisterType<GetTransactionsResponse>("FpResponses", 1, 0, "GetTransactionsResponse");
    qmlRegisterType<GetQuotesResponse>("FpResponses", 1, 0, "GetQuotesResponse");
    qmlRegisterType<GetFundamentalsResponse>("FpResponses", 1, 0, "GetFundamentalsResponse");
    qmlRegisterType<GetCandlesResponse>("FpResponses", 1, 0, "GetCandlesResponse");
    qmlRegisterType<SymbolSearchResponse>("FpResponses", 1, 0, "SymbolSearchResponse");
    qmlRegisterType<GetShareDetailsResponse>("FpResponses", 1, 0, "GetShareDetailsResponse");
    qmlRegisterType<SendOrderResponse>("FpResponses", 1, 0, "SendOrderResponse");
    qmlRegisterType<GetLeaderboardResponse>("FpResponses", 1, 0, "GetLeaderboardResponse");
    qmlRegisterType<GetLeaderboardMeResponse>("FpResponses", 1, 0, "GetLeaderboardMeResponse");

    qmlRegisterType<BrokerageCostCalculator>("com.example.fpx", 1, 0, "BrokerageCostCalculator");
    qmlRegisterType<FinishNotifier>("com.example.fpx", 1, 0, "FinishNotifier");
    qmlRegisterType<FpSymbolSearchWrapper>("com.example.fpx", 1, 0, "FpSymbolSearchWrapper");
    qmlRegisterType<FpSymbolSearchResultsItemModel>("com.example.fpx", 1, 0,
                                                    "FpSymbolSearchResultsItemModel");
    qmlRegisterType<FpAccountsListModel>("com.example.fpx", 1, 0, "FpAccountsListModel");
    qmlRegisterType<FpPositionsListModel>("com.example.fpx", 1, 0, "FpPositionsListModel");
    qmlRegisterType<FpPortfolioWrapper>("com.example.fpx", 1, 0, "FpPortfolioWrapper");
    qmlRegisterType<FpChartDataWrapper>("com.example.fpx", 1, 0, "FpChartDataWrapper");
    qmlRegisterType<FpLeaderboardListModel>("com.example.fpx", 1, 0, "FpLeaderboardListModel");
    qmlRegisterType<FpLeaderboardWrapper>("com.example.fpx", 1, 0, "FpLeaderboardWrapper");
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

    UtilityFunctionsWrapper utilityFunctions;

    auto client = bsmi::InvestorAPIClientFactory::create(
        QStringLiteral("https://investor-api.herokuapp.com"));
    FpSettings fpCoreSettings;
    FpCore fpCore(client, &fpCoreSettings);
    client->setParent(&fpCore);
    FpLocale fpLocale;
    FalseProfitsDeclarativeTypes fpTypes;

    QQmlApplicationEngine engine;
    if (!(isHostMobile() || app.arguments().contains(QLatin1String("-touch")))) {
        QQmlFileSelector::get(&engine)->setExtraSelectors({ QLatin1String("desktop") });
    }
    engine.rootContext()->setContextProperty("utilityFunctions", &utilityFunctions);
    engine.rootContext()->setContextProperty("fpCore", &fpCore);
    engine.rootContext()->setContextProperty("fpLocale", &fpLocale);
    engine.rootContext()->setContextProperty("fpType", &fpTypes);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    fpCore.loadLaunchSettings();

    return app.exec();
}

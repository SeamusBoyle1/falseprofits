// Copyright 2017

#ifndef FPDECLARATIVETYPES_H
#define FPDECLARATIVETYPES_H

#include <QObject>

#include <QDateTime>
#include <QJsonArray>
#include <QJsonObject>
#include <QVariant>
#include <QJsonDocument>

class NewUserDetails
{
    Q_GADGET
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName)
    Q_PROPERTY(QString email READ email WRITE setEmail)
    Q_PROPERTY(QString password READ password WRITE setPassword)
public:
    QString displayName() const { return m_displayName; }
    void setDisplayName(QString displayName) { m_displayName = displayName; }

    QString email() const { return m_email; }
    void setEmail(QString email) { m_email = email; }

    QString password() const { return m_password; }
    void setPassword(QString password) { m_password = password; }

private:
    QString m_displayName;
    QString m_email;
    QString m_password;
};

class CandlesRequestArgs
{
private:
    Q_GADGET
    Q_PROPERTY(QString symbol READ symbol WRITE setSymbol)
    Q_PROPERTY(QDateTime startTime READ startTime WRITE setStartTime)
    Q_PROPERTY(QDateTime endTime READ endTime WRITE setEndTime)
    Q_PROPERTY(QString interval READ interval WRITE setInterval)
    Q_PROPERTY(QString range READ range WRITE setRange)
public:
    QString symbol() const { return m_symbol; }

    void setSymbol(QString symbol) { m_symbol = symbol; }

    QDateTime startTime() const { return m_startTime; }

    void setStartTime(QDateTime startTime) { m_startTime = startTime; }

    QDateTime endTime() const { return m_endTime; }

    void setEndTime(QDateTime endTime) { m_endTime = endTime; }

    QString interval() const { return m_interval; }

    void setInterval(QString interval) { m_interval = interval; }

    QString range() const { return m_range; }

    void setRange(QString range) { m_range = range; }

private:
    QString m_symbol;
    QDateTime m_startTime;
    QDateTime m_endTime;
    QString m_interval;
    QString m_range;
};

class TransactionsQuery
{
    Q_GADGET
    Q_PROPERTY(QString accountId READ accountId WRITE setAccountId)
    Q_PROPERTY(QDateTime startDate READ startDate WRITE setStartDate)
    Q_PROPERTY(QDateTime endDate READ endDate WRITE setEndDate)
    Q_PROPERTY(int pageNumber READ pageNumber WRITE setPageNumber)
    Q_PROPERTY(int pageSize READ pageSize WRITE setPageSize)
public:
    QString accountId() const { return m_accountId; }

    void setAccountId(QString accountId) { m_accountId = accountId; }

    QDateTime startDate() const { return m_startDate; }

    void setStartDate(QDateTime startDate) { m_startDate = startDate; }

    QDateTime endDate() const { return m_endDate; }

    void setEndDate(QDateTime endDate) { m_endDate = endDate; }

    int pageNumber() const { return m_pageNumber; }

    void setPageNumber(int pageNumber) { m_pageNumber = pageNumber; }

    int pageSize() const { return m_pageSize; }

    void setPageSize(int pageSize) { m_pageSize = pageSize; }

private:
    QString m_accountId;
    QDateTime m_startDate;
    QDateTime m_endDate;
    int m_pageNumber{ -1 };
    int m_pageSize{ -1 };
};

class SymbolSearchQuery
{
    Q_GADGET
    Q_PROPERTY(QString searchTerm READ searchTerm WRITE setSearchTerm)
    Q_PROPERTY(int pageNumber READ pageNumber WRITE setPageNumber)
    Q_PROPERTY(int pageSize READ pageSize WRITE setPageSize)
public:
    QString searchTerm() const { return m_searchTerm; }

    void setSearchTerm(QString searchTerm) { m_searchTerm = searchTerm; }

    int pageNumber() const { return m_pageNumber; }

    void setPageNumber(int pageNumber) { m_pageNumber = pageNumber; }

    int pageSize() const { return m_pageSize; }

    void setPageSize(int pageSize) { m_pageSize = pageSize; }

private:
    QString m_searchTerm;
    int m_pageNumber{ -1 };
    int m_pageSize{ -1 };
};

class OrderParams
{
public:
    enum class Side { BuySide, SellSide };
    Q_ENUM(Side)

private:
    Q_GADGET
    Q_PROPERTY(QString symbol READ symbol WRITE setSymbol)
    Q_PROPERTY(int quantity READ quantity WRITE setQuantity)
    Q_PROPERTY(Side side READ side WRITE setSide)
    Q_PROPERTY(qint64 nonce READ nonce WRITE setNonce)
public:

    QString symbol() const { return m_symbol; }

    void setSymbol(QString symbol) { m_symbol = symbol; }

    int quantity() const { return m_quantity; }

    void setQuantity(int quantity) { m_quantity = quantity; }

    Side side() const { return m_side; }

    void setSide(Side side) { m_side = side; }

    qint64 nonce() const { return m_nonce; }

    void setNonce(qint64 nonce) { m_nonce = nonce; }

private:
    QString m_symbol;
    qint64 m_nonce{ 0 };
    int m_quantity{ 0 };
    Side m_side{ Side::BuySide };
};

class BrokerCostCalcArgs
{
    Q_GADGET
    Q_PROPERTY(int quantity READ quantity WRITE setQuantity)
    Q_PROPERTY(double price READ price WRITE setPrice)
    Q_PROPERTY(OrderParams::Side side READ side WRITE setSide)
public:
    int quantity() const { return m_quantity; }

    void setQuantity(int quantity) { m_quantity = quantity; }

    double price() const { return m_price; }

    void setPrice(double price) { m_price = price; }

    OrderParams::Side side() const { return m_side; }

    void setSide(OrderParams::Side side) { m_side = side; }

private:
    double m_price{ 0 };
    int m_quantity{ 0 };
    OrderParams::Side m_side;
};

class BrokerCostCalcResult
{
    Q_GADGET
    Q_PROPERTY(double orderValue READ orderValue WRITE setOrderValue)
    Q_PROPERTY(double brokerageCost READ brokerageCost WRITE setBrokerageCost)
    Q_PROPERTY(double estimatedTotal READ estimatedTotal WRITE setEstimatedTotal)
public:
    double orderValue() const { return m_orderValue; }

    void setOrderValue(double orderValue) { m_orderValue = orderValue; }

    double brokerageCost() const { return m_brokerageCost; }

    void setBrokerageCost(double brokerageCost) { m_brokerageCost = brokerageCost; }

    double estimatedTotal() const { return m_estimatedTotal; }

    void setEstimatedTotal(double estimatedTotal) { m_estimatedTotal = estimatedTotal; }

private:
    double m_orderValue{ 0 };
    double m_brokerageCost{ 0 };
    double m_estimatedTotal{ 0 };
};

class JsonUserTradingAccount
{
    Q_GADGET
    Q_PROPERTY(QVariant id READ id)
    Q_PROPERTY(QVariant name READ name)
    Q_PROPERTY(QVariant balance READ balance)
public:
    QJsonObject m_d;

    QVariant id() const { return m_d.value(QLatin1String("id")).toVariant(); }

    QVariant name() const { return m_d.value(QLatin1String("name")).toVariant(); }

    QVariant balance() const { return m_d.value(QLatin1String("balance")).toVariant(); }
};

class JsonUserTradingAccounts
{
    Q_GADGET
    Q_PROPERTY(bool isEmpty READ isEmpty)
    Q_PROPERTY(int size READ size)
public:
    QJsonArray m_d;

    bool isEmpty() const { return m_d.isEmpty(); }

    int size() const { return m_d.size(); }

    Q_INVOKABLE
    JsonUserTradingAccount at(int i) const { return { m_d.at(i).toObject() }; }

    Q_INVOKABLE
    JsonUserTradingAccount find(const QString &accountId) const
    {
        auto const idJsonVal = QJsonValue(accountId);
        auto it =
            std::find_if(m_d.constBegin(), m_d.constEnd(), [idJsonVal](const QJsonValue &a) {
                auto const obj = a.toObject();
                return obj.value(QLatin1String("id")) == idJsonVal;
            });
        if (it != m_d.constEnd()) {
            return { it->toObject() };
        }
        return JsonUserTradingAccount();
    }
};

class JsonUserDetails
{
    Q_GADGET
    Q_PROPERTY(QVariant id READ id)
    Q_PROPERTY(QVariant email READ email)
    Q_PROPERTY(QVariant displayName READ displayName)
    Q_PROPERTY(QVariant level READ level)
    Q_PROPERTY(JsonUserTradingAccounts accounts READ accounts)
public:
    QJsonObject m_d;

    QVariant id() const { return m_d.value(QLatin1String("id")).toVariant(); }

    QVariant email() const { return m_d.value(QLatin1String("email")).toVariant(); }

    QVariant displayName() const { return m_d.value(QLatin1String("displayName")).toVariant(); }

    QVariant level() const { return m_d.value(QLatin1String("level")).toVariant(); }

    JsonUserTradingAccounts accounts() const
    {
        return { m_d.value(QLatin1String("accounts")).toArray() };
    }
};

class JsonQuotesQuote
{
    Q_GADGET
    Q_PROPERTY(bool isValid READ isValid)
    Q_PROPERTY(QVariant symbol READ symbol)
    Q_PROPERTY(QVariant ask READ ask)
    Q_PROPERTY(QVariant askSize READ askSize)
    Q_PROPERTY(QVariant bid READ bid)
    Q_PROPERTY(QVariant bidSize READ bidSize)
    Q_PROPERTY(QVariant last READ last)
    Q_PROPERTY(QVariant lastSize READ lastSize)
    Q_PROPERTY(QVariant change READ change)
    Q_PROPERTY(QVariant changePercent READ changePercent)
    Q_PROPERTY(QVariant dayLow READ dayLow)
    Q_PROPERTY(QVariant dayHigh READ dayHigh)
public:
    QJsonObject m_d;

    bool isValid() const { return !m_d.isEmpty() && m_d.contains(QLatin1String("symbol")); }

    QVariant symbol() const { return m_d.value(QLatin1String("symbol")).toVariant(); }

    QVariant ask() const { return m_d.value(QLatin1String("ask")).toVariant(); }

    QVariant askSize() const { return m_d.value(QLatin1String("askSize")).toVariant(); }

    QVariant bid() const { return m_d.value(QLatin1String("bid")).toVariant(); }

    QVariant bidSize() const { return m_d.value(QLatin1String("bidSize")).toVariant(); }

    QVariant last() const { return m_d.value(QLatin1String("last")).toVariant(); }

    QVariant lastSize() const { return m_d.value(QLatin1String("lastSize")).toVariant(); }

    QVariant change() const { return m_d.value(QLatin1String("change")).toVariant(); }

    QVariant changePercent() const { return m_d.value(QLatin1String("changePercent")).toVariant(); }

    QVariant dayLow() const { return m_d.value(QLatin1String("dayLow")).toVariant(); }

    QVariant dayHigh() const { return m_d.value(QLatin1String("dayHigh")).toVariant(); }
};

class JsonQuotes
{
    Q_GADGET
    Q_PROPERTY(bool isEmpty READ isEmpty)
    Q_PROPERTY(int size READ size)
public:
    QJsonArray m_d;

    bool isEmpty() const { return m_d.isEmpty(); }

    int size() const { return m_d.size(); }

    Q_INVOKABLE
    JsonQuotesQuote at(int i) const { return { m_d.at(i).toObject() }; }

    Q_INVOKABLE
    JsonQuotesQuote find(const QString &symbol) const
    {
        auto const symbolJsonVal = QJsonValue(symbol);
        auto it =
            std::find_if(m_d.constBegin(), m_d.constEnd(), [symbolJsonVal](const QJsonValue &a) {
                auto const obj = a.toObject();
                return obj.value(QLatin1String("symbol")) == symbolJsonVal;
            });
        if (it != m_d.constEnd()) {
            return { it->toObject() };
        }
        return JsonQuotesQuote();
    }
};

class JsonSymbolSearchResult
{
    Q_GADGET
    Q_PROPERTY(bool isValid READ isValid)
    Q_PROPERTY(QVariant symbol READ symbol)
    Q_PROPERTY(QVariant name READ name)
    Q_PROPERTY(QVariant industry READ industry)
public:
    QJsonObject m_d;

    bool isValid() const { return !m_d.isEmpty() && m_d.contains(QLatin1String("symbol")); }

    QVariant symbol() const { return m_d.value(QLatin1String("symbol")).toVariant(); }

    QVariant name() const { return m_d.value(QLatin1String("name")).toVariant(); }

    QVariant industry() const { return m_d.value(QLatin1String("industry")).toVariant(); }
};

class JsonSymbolSearchResults
{
    Q_GADGET
    Q_PROPERTY(bool isEmpty READ isEmpty)
    Q_PROPERTY(int size READ size)
    Q_PROPERTY(int pageNumber READ pageNumber)
    Q_PROPERTY(int pageSize READ pageSize)
    Q_PROPERTY(int totalPageCount READ totalPageCount)
    Q_PROPERTY(int totalRowCount READ totalRowCount)
public:
    JsonSymbolSearchResults() {}
    JsonSymbolSearchResults(QJsonObject o)
        : m_d{ std::move(o) }
    {
        m_items = o.value(QLatin1String("items")).toArray();
    }

    bool isEmpty() const { return m_items.isEmpty(); }

    int size() const { return m_items.size(); }

    Q_INVOKABLE
    JsonSymbolSearchResult at(int i) const { return { m_items.at(i).toObject() }; }

    int pageNumber() const { return m_d.value(QLatin1String("pageNumber")).toInt(); }

    int pageSize() const { return m_d.value(QLatin1String("pageSize")).toInt(); }

    int totalPageCount() const { return m_d.value(QLatin1String("totalPageCount")).toInt(); }

    int totalRowCount() const { return m_d.value(QLatin1String("totalRowCount")).toInt(); }

private:
    QJsonObject m_d;
    QJsonArray m_items;
};

Q_DECLARE_METATYPE(NewUserDetails)
Q_DECLARE_METATYPE(CandlesRequestArgs)
Q_DECLARE_METATYPE(TransactionsQuery)
Q_DECLARE_METATYPE(SymbolSearchQuery)
Q_DECLARE_METATYPE(OrderParams)
Q_DECLARE_METATYPE(BrokerCostCalcArgs)
Q_DECLARE_METATYPE(BrokerCostCalcResult)
Q_DECLARE_METATYPE(JsonUserTradingAccount)
Q_DECLARE_METATYPE(JsonUserTradingAccounts)
Q_DECLARE_METATYPE(JsonUserDetails)
Q_DECLARE_METATYPE(JsonQuotesQuote)
Q_DECLARE_METATYPE(JsonQuotes)
Q_DECLARE_METATYPE(JsonSymbolSearchResult)
Q_DECLARE_METATYPE(JsonSymbolSearchResults)

class FalseProfitsDeclarativeTypes : public QObject
{
    Q_OBJECT
public:
    explicit FalseProfitsDeclarativeTypes(QObject *parent = nullptr)
        : QObject(parent)
    {
        qRegisterMetaType<NewUserDetails>("NewUserDetails");
        qRegisterMetaType<CandlesRequestArgs>("CandlesRequestArgs");
        qRegisterMetaType<TransactionsQuery>("TransactionsQuery");
        qRegisterMetaType<SymbolSearchQuery>("SymbolSearchQuery");
        qRegisterMetaType<OrderParams>("OrderParams");
        qRegisterMetaType<OrderParams::Side>("OrderParams::Side");
        qRegisterMetaType<BrokerCostCalcArgs>("BrokerCostCalcArgs");
        qRegisterMetaType<BrokerCostCalcResult>("BrokerCostCalcResult");

        qRegisterMetaType<JsonUserTradingAccount>("JsonUserTradingAccount");
        qRegisterMetaType<JsonUserTradingAccounts>("JsonUserTradingAccounts");
        qRegisterMetaType<JsonUserDetails>("JsonUserDetails");
        qRegisterMetaType<JsonQuotesQuote>("JsonQuotesQuote");
        qRegisterMetaType<JsonQuotes>("JsonQuotes");
        qRegisterMetaType<JsonSymbolSearchResult>("JsonSymbolSearchResult");
        qRegisterMetaType<JsonSymbolSearchResults>("JsonSymbolSearchResults");
    }

    Q_INVOKABLE
    NewUserDetails makeNewUserDetails() { return NewUserDetails{}; }

    Q_INVOKABLE
    CandlesRequestArgs makeCandlesRequestArgs() { return CandlesRequestArgs{}; }

    Q_INVOKABLE
    TransactionsQuery makeTransactionsQuery() { return TransactionsQuery{}; }

    Q_INVOKABLE
    SymbolSearchQuery makeSymbolSearchQuery() { return SymbolSearchQuery{}; }

    Q_INVOKABLE
    OrderParams makeOrderParams() { return OrderParams{}; }

    Q_INVOKABLE
    BrokerCostCalcArgs makeBrokerCostCalcArgs() { return BrokerCostCalcArgs{}; }

    Q_INVOKABLE
    JsonUserDetails makeJsonUserDetails(const QByteArray &json)
    {
        return { QJsonDocument::fromJson(json).object() };
    }

    Q_INVOKABLE
    JsonQuotes makeJsonQuotes(const QByteArray &json)
    {
        return { QJsonDocument::fromJson(json).array() };
    }

    Q_INVOKABLE
    JsonSymbolSearchResults makeJsonSymbolSearchResults(const QByteArray &json)
    {
        return { QJsonDocument::fromJson(json).object() };
    }
};

#endif // FPDECLARATIVETYPES_H

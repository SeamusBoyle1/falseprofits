// Copyright 2017

#include <QString>
#include <QtTest>

#include <InvestorAPIClient/requestqueue.h>

#include <QJsonObject>
#include <QNetworkAccessManager>

class RequestQueueTest : public QObject
{
    Q_OBJECT

public:
    RequestQueueTest();

private Q_SLOTS:
    void testCase1();
};

RequestQueueTest::RequestQueueTest()
{
}

void RequestQueueTest::testCase1()
{
    using namespace bsmi;

    {
        QNetworkAccessManager nam;
        nam.setNetworkAccessible(QNetworkAccessManager::NotAccessible);
        RequestQueue q(&nam);

        QNetworkRequest req1(QUrl("http://localhost"));

        auto rep1get = q.get(req1);
        QCOMPARE(rep1get->operation(), QNetworkAccessManager::GetOperation);

        QJsonObject payload1;
        auto rep1post = q.post(req1, payload1);
        QCOMPARE(rep1post->operation(), QNetworkAccessManager::PostOperation);

        auto rep1delete = q.deleteResource(req1);
        QCOMPARE(rep1delete->operation(), QNetworkAccessManager::DeleteOperation);
    }
}

QTEST_MAIN(RequestQueueTest)

#include "tst_requestqueuetest.moc"

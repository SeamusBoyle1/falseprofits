// Copyright 2017

#include <QString>
#include <QtTest>

class FpCoreTest : public QObject
{
    Q_OBJECT

public:
    FpCoreTest();

private Q_SLOTS:
    void testCase1();
};

FpCoreTest::FpCoreTest()
{
}

void FpCoreTest::testCase1()
{
    QVERIFY2(true, "Failure");
}

QTEST_APPLESS_MAIN(FpCoreTest)

#include "tst_fpcoretest.moc"

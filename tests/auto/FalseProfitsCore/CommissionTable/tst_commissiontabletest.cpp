// Copyright 2017

#include <QString>
#include <QtTest>

#include <FalseProfitsCore/commissiontable.h>

class CommissionTableTest : public QObject
{
    Q_OBJECT

public:
    CommissionTableTest();

private Q_SLOTS:
    void testCase1();
    void testCase2();
};

CommissionTableTest::CommissionTableTest()
{
}

void CommissionTableTest::testCase1()
{
    // Tests single fixed row, and single percent row

    CommissionTable comm;

    comm.insertFixed(1000000, 50);

    comm.insertPercent(1000000, 1);

    comm.sort();

    // quantity below max of inserted row
    auto aQty = 20;
    auto aFixed = comm.fixedCommission(aQty);
    QVERIFY(aFixed);
    QCOMPARE(*aFixed, 50.0);

    auto aPercent = comm.percentCommission(aQty);
    QVERIFY(aPercent);
    QCOMPARE(*aPercent, 1.0);

    // quantity equal to max of inserted rows
    // max value is inclusive
    auto bQty = 1000000;
    auto bFixed = comm.fixedCommission(bQty);
    QVERIFY(bFixed);
    QCOMPARE(*bFixed, 50.0);

    auto bPercent = comm.percentCommission(bQty);
    QVERIFY(bPercent);
    QCOMPARE(*bPercent, 1.0);

    // quantity equal to max less one of inserted rows
    // max value is inclusive
    auto cQty = 999999;
    auto cFixed = comm.fixedCommission(cQty);
    QVERIFY(cFixed);
    QCOMPARE(*cFixed, 50.0);

    auto cPercent = comm.percentCommission(cQty);
    QVERIFY(cPercent);
    QCOMPARE(*cPercent, 1.0);

    // quantity creater than max of inserted rows
    auto dQty = 1000001;
    auto dFixed = comm.fixedCommission(dQty);
    QVERIFY(!dFixed);

    auto dPercent = comm.percentCommission(dQty);
    QVERIFY(!dPercent);
}

void CommissionTableTest::testCase2()
{
    // Tests multiple (four) fixed and percent rows,
    // with different quantity ranges

    CommissionTable comm;

    comm.insertFixed(100, 50);
    comm.insertFixed(200, 51);
    comm.insertFixed(300, 52);
    comm.insertFixed(400, 53);

    comm.insertPercent(99, 1);
    comm.insertPercent(199, 2);
    comm.insertPercent(299, 3);
    comm.insertPercent(399, 4);

    comm.sort();

    {
        // One below max in fixed row
        auto aQty = 99;
        auto aFixed = comm.fixedCommission(aQty);
        QVERIFY(aFixed);
        QCOMPARE(*aFixed, 50.0);

        auto bQty = 199;
        auto bFixed = comm.fixedCommission(bQty);
        QVERIFY(bFixed);
        QCOMPARE(*bFixed, 51.0);

        auto cQty = 299;
        auto cFixed = comm.fixedCommission(cQty);
        QVERIFY(cFixed);
        QCOMPARE(*cFixed, 52.0);

        auto dQty = 399;
        auto dFixed = comm.fixedCommission(dQty);
        QVERIFY(dFixed);
        QCOMPARE(*dFixed, 53.0);

        // qty greater than max of largest fixed range
        auto eQty = 499;
        auto eFixed = comm.fixedCommission(eQty);
        QVERIFY(!eFixed);
    }

    {
        // Equal to max in fixed row
        auto aQty = 100;
        auto aFixed = comm.fixedCommission(aQty);
        QVERIFY(aFixed);
        QCOMPARE(*aFixed, 50.0);

        auto bQty = 200;
        auto bFixed = comm.fixedCommission(bQty);
        QVERIFY(bFixed);
        QCOMPARE(*bFixed, 51.0);

        auto cQty = 300;
        auto cFixed = comm.fixedCommission(cQty);
        QVERIFY(cFixed);
        QCOMPARE(*cFixed, 52.0);

        // qty equal to max of largest fixed range
        // max is inclusive
        auto dQty = 400;
        auto dFixed = comm.fixedCommission(dQty);
        QVERIFY(dFixed);
        QCOMPARE(*dFixed, 53.0);

        // qty greater than max of largest fixed range
        auto eQty = 401;
        auto eFixed = comm.fixedCommission(eQty);
        QVERIFY(!eFixed);
    }

    {
        // One below max in percent row
        auto aQty = 98;
        auto aPct = comm.percentCommission(aQty);
        QVERIFY(aPct);
        QCOMPARE(*aPct, 1.0);

        auto bQty = 198;
        auto bPct = comm.percentCommission(bQty);
        QVERIFY(bPct);
        QCOMPARE(*bPct, 2.0);

        auto cQty = 298;
        auto cPct = comm.percentCommission(cQty);
        QVERIFY(cPct);
        QCOMPARE(*cPct, 3.0);

        auto dQty = 398;
        auto dPct = comm.percentCommission(dQty);
        QVERIFY(dPct);
        QCOMPARE(*dPct, 4.0);

        // qty greater than max of largest percent range
        auto eQty = 498;
        auto ePct = comm.percentCommission(eQty);
        QVERIFY(!ePct);
    }

    {
        // Equal to max in percent row
        auto aQty = 99;
        auto aPct = comm.percentCommission(aQty);
        QVERIFY(aPct);
        QCOMPARE(*aPct, 1.0);

        auto bQty = 199;
        auto bPct = comm.percentCommission(bQty);
        QVERIFY(bPct);
        QCOMPARE(*bPct, 2.0);

        auto cQty = 299;
        auto cPct = comm.percentCommission(cQty);
        QVERIFY(cPct);
        QCOMPARE(*cPct, 3.0);

        // qty equal to max of largest percent range
        // max is inclusive
        auto dQty = 399;
        auto dPct = comm.percentCommission(dQty);
        QVERIFY(dPct);
        QCOMPARE(*dPct, 4.0);

        // qty equal to max of largest percent range
        auto eQty = 400;
        auto ePct = comm.percentCommission(eQty);
        QVERIFY(!ePct);
    }
}

QTEST_APPLESS_MAIN(CommissionTableTest)

#include "tst_commissiontabletest.moc"

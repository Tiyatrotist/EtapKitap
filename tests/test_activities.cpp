#include <QtTest/QtTest>
#include "activities/MultipleChoiceActivity.h"
#include "activities/MatchingActivity.h"
#include "activities/ActivityFactory.h"

using namespace EtapKitap;

class TestActivities : public QObject {
    Q_OBJECT

private slots:
    void testMultipleChoiceEvaluation();
    void testMatchingEvaluation();
    void testActivityFactory();
};

void TestActivities::testMultipleChoiceEvaluation() {
    MultipleChoiceActivity mcq;
    mcq.setTitle(QStringLiteral("Kümeler Testi"));
    mcq.setQuestionText(QStringLiteral("A = {1, 2} kümesinin alt küme sayısı kaçtır?"));
    mcq.setOptions({QStringLiteral("2"), QStringLiteral("4"), QStringLiteral("8"), QStringLiteral("16")});
    mcq.setCorrectIndex(1); // Answer 4

    QVERIFY(!mcq.isAnswered());
    mcq.selectOption(0);
    QVERIFY(mcq.isAnswered());
    QVERIFY(!mcq.validate());
    QCOMPARE(mcq.score(), 0.0);

    mcq.selectOption(1);
    QVERIFY(mcq.validate());
    QCOMPARE(mcq.score(), 1.0);
}

void TestActivities::testMatchingEvaluation() {
    MatchingActivity match;
    match.addPair(QStringLiteral("Helyum"), QStringLiteral("He"));
    match.addPair(QStringLiteral("Oksijen"), QStringLiteral("O"));

    QVERIFY(!match.isAnswered());
    match.match(0, 0);
    match.match(1, 1);
    QVERIFY(match.isAnswered());
    QVERIFY(match.validate());
    QCOMPARE(match.score(), 1.0);
}

void TestActivities::testActivityFactory() {
    const QByteArray json = R"({
        "type": "multiple_choice",
        "id": "act-1",
        "title": "Fizik Kuvvet Testi",
        "question": "F = m * a formülü kime aittir?",
        "options": ["Newton", "Einstein", "Galileo"],
        "correctIndex": 0
    })";

    QString err;
    auto act = ActivityFactory::createFromJson(json, &err);
    QVERIFY2(act != nullptr, qPrintable(err));
    QCOMPARE(act->id(), QStringLiteral("act-1"));
    QCOMPARE(act->type(), ActivityType::MultipleChoice);

    auto* mcq = dynamic_cast<MultipleChoiceActivity*>(act.get());
    QVERIFY(mcq != nullptr);
    QCOMPARE(mcq->correctIndex(), 0);
    mcq->selectOption(0);
    QVERIFY(mcq->validate());
}

QTEST_MAIN(TestActivities)
#include "test_activities.moc"

#include <QtTest/QtTest>
#include "annotations/StrokeAnnotation.h"
#include "annotations/ShapeAnnotation.h"
#include "annotations/AnnotationModel.h"
#include "annotations/AnnotationCommands.h"
#include "annotations/UndoManager.h"

using namespace EtapKitap;

class TestAnnotations : public QObject {
    Q_OBJECT

private slots:
    void testStrokeBoundingBox();
    void testStrokeIntersection();
    void testShapeAnnotation();
    void testUndoRedo();
};

void TestAnnotations::testStrokeBoundingBox() {
    auto stroke = std::make_shared<StrokeAnnotation>(1, ToolType::Pen);
    stroke->setWidth(0.01);
    stroke->addPoint(NormalizedPoint{0.2, 0.2});
    stroke->addPoint(NormalizedPoint{0.4, 0.6});

    auto rect = stroke->boundingRect();
    QVERIFY(rect.isValid());
    QVERIFY(rect.x <= 0.2);
    QVERIFY(rect.y <= 0.2);
    QVERIFY((rect.x + rect.width) >= 0.4);
    QVERIFY((rect.y + rect.height) >= 0.6);
}

void TestAnnotations::testStrokeIntersection() {
    auto stroke = std::make_shared<StrokeAnnotation>(1, ToolType::Pen);
    stroke->setWidth(0.01);
    stroke->addPoint(NormalizedPoint{0.1, 0.1});
    stroke->addPoint(NormalizedPoint{0.5, 0.1}); // Horizontal line y=0.1

    // Touch directly on the line
    QVERIFY(stroke->intersects(NormalizedPoint{0.3, 0.1}, 0.01));
    // Touch near the line
    QVERIFY(stroke->intersects(NormalizedPoint{0.3, 0.11}, 0.02));
    // Touch far away
    QVERIFY(!stroke->intersects(NormalizedPoint{0.3, 0.5}, 0.01));
}

void TestAnnotations::testShapeAnnotation() {
    auto rectShape = std::make_shared<ShapeAnnotation>(1, ToolType::Rectangle);
    rectShape->setStartPoint(NormalizedPoint{0.1, 0.1});
    rectShape->setEndPoint(NormalizedPoint{0.5, 0.5});

    auto bbox = rectShape->boundingRect();
    QVERIFY(bbox.isValid());
    QVERIFY(rectShape->intersects(NormalizedPoint{0.2, 0.2}, 0.01));

    QJsonObject json = rectShape->toJson();
    auto restored = ShapeAnnotation::fromJson(json);
    QVERIFY(restored != nullptr);
    QCOMPARE(restored->pageNumber(), 1);
    QCOMPARE(restored->type(), ToolType::Rectangle);
}

void TestAnnotations::testUndoRedo() {
    AnnotationModel model;
    UndoManager undoMgr;

    auto stroke = std::make_shared<StrokeAnnotation>(1, ToolType::Pen);
    stroke->addPoint(NormalizedPoint{0.1, 0.1});
    stroke->addPoint(NormalizedPoint{0.2, 0.2});

    auto cmd = new AddAnnotationCommand(&model, stroke);
    undoMgr.push(cmd);

    QCOMPARE(model.countForPage(1), 1);
    QVERIFY(undoMgr.canUndo());

    undoMgr.undo();
    QCOMPARE(model.countForPage(1), 0);
    QVERIFY(undoMgr.canRedo());

    undoMgr.redo();
    QCOMPARE(model.countForPage(1), 1);
}

QTEST_MAIN(TestAnnotations)
#include "test_annotations.moc"

#pragma once

#include "annotations/Annotation.h"
#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QVector>
#include <QtGui/QPainter>
#include <memory>

namespace EtapKitap {

class AnnotationModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(int totalAnnotationsCount READ totalAnnotationsCount NOTIFY countChanged)

public:
    explicit AnnotationModel(QObject* parent = nullptr);
    ~AnnotationModel() override = default;

    void addAnnotation(std::shared_ptr<Annotation> annotation);
    bool removeAnnotation(const QString& id);
    void clearPage(int pageNumber);
    void clearAll();

    [[nodiscard]] QVector<std::shared_ptr<Annotation>> annotationsForPage(int pageNumber) const;
    [[nodiscard]] int countForPage(int pageNumber) const;
    [[nodiscard]] int totalAnnotationsCount() const;

    [[nodiscard]] std::shared_ptr<Annotation> findIntersecting(int pageNumber, const NormalizedPoint& point, qreal tolerance) const;

    void renderPageAnnotations(QPainter& painter, int pageNumber, qreal pageWidth, qreal pageHeight) const;

    [[nodiscard]] bool isVisible() const;
    void setVisible(bool visible);
    Q_INVOKABLE void toggleVisible();

    [[nodiscard]] QMap<int, QVector<std::shared_ptr<Annotation>>> allAnnotations() const;
    void loadAnnotations(const QVector<std::shared_ptr<Annotation>>& list);

signals:
    void visibleChanged(bool visible);
    void countChanged();
    void pageAnnotationsChanged(int pageNumber);

private:
    QMap<int, QVector<std::shared_ptr<Annotation>>> m_pageAnnotations;
    QMap<QString, std::shared_ptr<Annotation>> m_idMap;
    bool m_visible{true};
};

} // namespace EtapKitap

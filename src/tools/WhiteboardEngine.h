#pragma once

#include "core/Types.h"
#include "annotations/AnnotationModel.h"
#include <QtCore/QObject>
#include <vector>
#include <QtGui/QPainter>
#include <memory>

namespace EtapKitap {

class WhiteboardEngine : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(int currentTemplate READ currentTemplateAsInt WRITE setCurrentTemplateAsInt NOTIFY templateChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(int pageCount READ pageCount NOTIFY pageCountChanged)

public:
    explicit WhiteboardEngine(QObject* parent = nullptr);
    ~WhiteboardEngine() override = default;

    [[nodiscard]] bool isActive() const;
    void setActive(bool active);

    [[nodiscard]] WhiteboardTemplate currentTemplate() const;
    void setCurrentTemplate(WhiteboardTemplate templ);
    [[nodiscard]] int currentTemplateAsInt() const;
    void setCurrentTemplateAsInt(int t);

    [[nodiscard]] int currentPage() const;
    void setCurrentPage(int p);
    [[nodiscard]] int pageCount() const;

    [[nodiscard]] AnnotationModel* annotationsForPage(int page);
    [[nodiscard]] AnnotationModel* currentAnnotations();

    Q_INVOKABLE void addPage();
    Q_INVOKABLE void nextPage();
    Q_INVOKABLE void previousPage();
    Q_INVOKABLE void clearCurrentPage();

    void renderBackground(QPainter& painter, qreal width, qreal height) const;

signals:
    void activeChanged(bool active);
    void templateChanged(WhiteboardTemplate templ);
    void currentPageChanged(int page);
    void pageCountChanged(int count);

private:
    bool m_active{false};
    WhiteboardTemplate m_template{WhiteboardTemplate::Grid};
    int m_currentPage{1};
    std::vector<std::unique_ptr<AnnotationModel>> m_pages;
};

} // namespace EtapKitap

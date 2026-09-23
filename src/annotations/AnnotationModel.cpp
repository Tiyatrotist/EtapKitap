#include "annotations/AnnotationModel.h"
#include "core/Logger.h"

namespace EtapKitap {

AnnotationModel::AnnotationModel(QObject* parent)
    : QObject(parent)
{
}

void AnnotationModel::addAnnotation(std::shared_ptr<Annotation> annotation) {
    if (!annotation) return;

    int page = annotation->pageNumber();
    m_pageAnnotations[page].append(annotation);
    m_idMap[annotation->id()] = annotation;

    emit pageAnnotationsChanged(page);
    emit countChanged();
}

bool AnnotationModel::removeAnnotation(const QString& id) {
    auto it = m_idMap.find(id);
    if (it == m_idMap.end()) {
        return false;
    }

    int page = it.value()->pageNumber();
    m_idMap.erase(it);

    auto& list = m_pageAnnotations[page];
    list.erase(std::remove_if(list.begin(), list.end(), [&](const auto& ann) {
        return ann->id() == id;
    }), list.end());

    emit pageAnnotationsChanged(page);
    emit countChanged();
    return true;
}

void AnnotationModel::clearPage(int pageNumber) {
    if (!m_pageAnnotations.contains(pageNumber)) {
        return;
    }

    for (const auto& ann : m_pageAnnotations[pageNumber]) {
        m_idMap.remove(ann->id());
    }
    m_pageAnnotations.remove(pageNumber);

    emit pageAnnotationsChanged(pageNumber);
    emit countChanged();
}

void AnnotationModel::clearAll() {
    m_pageAnnotations.clear();
    m_idMap.clear();
    emit countChanged();
}

QVector<std::shared_ptr<Annotation>> AnnotationModel::annotationsForPage(int pageNumber) const {
    return m_pageAnnotations.value(pageNumber);
}

int AnnotationModel::countForPage(int pageNumber) const {
    return static_cast<int>(m_pageAnnotations.value(pageNumber).size());
}

int AnnotationModel::totalAnnotationsCount() const {
    return static_cast<int>(m_idMap.size());
}

std::shared_ptr<Annotation> AnnotationModel::findIntersecting(int pageNumber, const NormalizedPoint& point, qreal tolerance) const {
    if (!m_pageAnnotations.contains(pageNumber)) {
        return nullptr;
    }

    // Traverse in reverse order (topmost first)
    const auto& list = m_pageAnnotations[pageNumber];
    for (auto it = list.rbegin(); it != list.rend(); ++it) {
        if ((*it)->intersects(point, tolerance)) {
            return *it;
        }
    }
    return nullptr;
}

void AnnotationModel::renderPageAnnotations(QPainter& painter, int pageNumber, qreal pageWidth, qreal pageHeight) const {
    if (!m_visible || !m_pageAnnotations.contains(pageNumber)) {
        return;
    }

    const auto& list = m_pageAnnotations[pageNumber];
    for (const auto& ann : list) {
        ann->render(painter, pageWidth, pageHeight);
    }
}

bool AnnotationModel::isVisible() const {
    return m_visible;
}

void AnnotationModel::setVisible(bool visible) {
    if (m_visible != visible) {
        m_visible = visible;
        emit visibleChanged(m_visible);
    }
}

void AnnotationModel::toggleVisible() {
    setVisible(!m_visible);
}

QMap<int, QVector<std::shared_ptr<Annotation>>> AnnotationModel::allAnnotations() const {
    return m_pageAnnotations;
}

void AnnotationModel::loadAnnotations(const QVector<std::shared_ptr<Annotation>>& list) {
    clearAll();
    for (const auto& ann : list) {
        addAnnotation(ann);
    }
}

} // namespace EtapKitap

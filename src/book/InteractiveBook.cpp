#include "book/InteractiveBook.h"

namespace EtapKitap {

InteractiveBook::InteractiveBook(std::unique_ptr<BookPackage> package, IPdfBackend* backend)
    : m_package(std::move(package))
    , m_backend(backend)
    , m_interactions(std::make_unique<InteractionRegistry>())
{
    if (m_package && m_package->isOpen()) {
        m_interactions->loadFromManifest(m_package->manifest());
    }
}

QString InteractiveBook::id() const {
    return (m_package && m_package->isOpen()) ? m_package->manifest().id : QString();
}

QString InteractiveBook::title() const {
    return (m_package && m_package->isOpen()) ? m_package->manifest().title : QString();
}

QString InteractiveBook::filePath() const {
    return m_package ? m_package->packagePath() : QString();
}

int InteractiveBook::pageCount() const {
    return m_backend ? m_backend->pageCount() : 0;
}

QSizeF InteractiveBook::pageSize(int pageNumber) const {
    return m_backend ? m_backend->pageSize(pageNumber) : QSizeF(595.0, 842.0);
}

bool InteractiveBook::isInteractiveBook() const {
    return true;
}

QString InteractiveBook::pageText(int pageNumber) const {
    return m_backend ? m_backend->pageText(pageNumber) : QString();
}

BookPackage* InteractiveBook::package() const {
    return m_package.get();
}

InteractionRegistry* InteractiveBook::interactions() const {
    return m_interactions.get();
}

} // namespace EtapKitap

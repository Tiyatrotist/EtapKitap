#include "pdf/PdfPageCache.h"
#include "core/Logger.h"

namespace EtapKitap {

PdfPageCache::PdfPageCache(size_t maxMemoryBytes, QObject* parent)
    : QObject(parent)
    , m_maxMemoryBytes(maxMemoryBytes)
{
}

bool PdfPageCache::contains(int pageNumber, const QSize& size) const {
    QMutexLocker locker(&m_mutex);
    CachedPageKey key{pageNumber, size.width(), size.height()};
    return m_cacheMap.find(key) != m_cacheMap.end();
}

QImage PdfPageCache::get(int pageNumber, const QSize& size) {
    QMutexLocker locker(&m_mutex);
    CachedPageKey key{pageNumber, size.width(), size.height()};
    auto it = m_cacheMap.find(key);
    if (it == m_cacheMap.end()) {
        return QImage();
    }

    // Move to front of LRU list
    m_lruList.splice(m_lruList.begin(), m_lruList, it->second);
    return it->second->image;
}

QImage PdfPageCache::getAnyResolution(int pageNumber) const {
    QMutexLocker locker(&m_mutex);
    for (const auto& item : m_lruList) {
        if (item.key.pageNumber == pageNumber && !item.image.isNull()) {
            return item.image;
        }
    }
    return QImage();
}

void PdfPageCache::insert(int pageNumber, const QSize& size, const QImage& image) {
    if (image.isNull() || size.isEmpty()) {
        return;
    }

    QMutexLocker locker(&m_mutex);
    CachedPageKey key{pageNumber, size.width(), size.height()};

    size_t byteCount = static_cast<size_t>(image.sizeInBytes());

    // If already in cache, update and promote
    auto it = m_cacheMap.find(key);
    if (it != m_cacheMap.end()) {
        m_currentMemoryBytes -= it->second->byteCount;
        it->second->image = image;
        it->second->byteCount = byteCount;
        m_currentMemoryBytes += byteCount;
        m_lruList.splice(m_lruList.begin(), m_lruList, it->second);
        evictIfNeeded();
        return;
    }

    // Insert new item
    m_lruList.push_front(CacheItem{key, image, byteCount});
    m_cacheMap[key] = m_lruList.begin();
    m_currentMemoryBytes += byteCount;

    evictIfNeeded();
}

void PdfPageCache::clear() {
    QMutexLocker locker(&m_mutex);
    m_lruList.clear();
    m_cacheMap.clear();
    m_currentMemoryBytes = 0;
    qCDebug(logPdf) << "PdfPageCache cleared.";
}

void PdfPageCache::setMaxMemoryBytes(size_t bytes) {
    QMutexLocker locker(&m_mutex);
    m_maxMemoryBytes = bytes;
    evictIfNeeded();
}

size_t PdfPageCache::currentMemoryUsage() const {
    QMutexLocker locker(&m_mutex);
    return m_currentMemoryBytes;
}

void PdfPageCache::evictIfNeeded() {
    while (m_currentMemoryBytes > m_maxMemoryBytes && !m_lruList.empty()) {
        auto lastIt = std::prev(m_lruList.end());
        m_currentMemoryBytes -= lastIt->byteCount;
        m_cacheMap.erase(lastIt->key);
        m_lruList.pop_back();
    }
}

} // namespace EtapKitap

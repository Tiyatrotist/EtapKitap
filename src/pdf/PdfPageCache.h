#pragma once

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QMap>
#include <QtGui/QImage>
#include <list>

namespace EtapKitap {

struct CachedPageKey {
    int pageNumber{0};
    int width{0};
    int height{0};

    auto operator<=>(const CachedPageKey&) const = default;
};

class PdfPageCache : public QObject {
    Q_OBJECT

public:
    explicit PdfPageCache(size_t maxMemoryBytes = 128 * 1024 * 1024, QObject* parent = nullptr); // 128 MB default
    ~PdfPageCache() override = default;

    [[nodiscard]] bool contains(int pageNumber, const QSize& size) const;
    [[nodiscard]] QImage get(int pageNumber, const QSize& size);
    [[nodiscard]] QImage getAnyResolution(int pageNumber) const;
    void insert(int pageNumber, const QSize& size, const QImage& image);
    void clear();

    void setMaxMemoryBytes(size_t bytes);
    [[nodiscard]] size_t currentMemoryUsage() const;

private:
    void evictIfNeeded();

    mutable QMutex m_mutex;
    size_t m_maxMemoryBytes;
    size_t m_currentMemoryBytes{0};

    struct CacheItem {
        CachedPageKey key;
        QImage image;
        size_t byteCount{0};
    };

    std::list<CacheItem> m_lruList;
    // Map key -> iterator in list
    std::map<CachedPageKey, std::list<CacheItem>::iterator> m_cacheMap;
};

} // namespace EtapKitap

# EtapKitap

[![CI](https://github.com/Tiyatrotist/EtapKitap/actions/workflows/ci.yml/badge.svg)](https://github.com/Tiyatrotist/EtapKitap/actions/workflows/ci.yml)
[![License: Apache-2.0](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
[![Target Platform](https://img.shields.io/badge/Platform-Pardus%20ETAP%2023-red.svg)](https://etap.org.tr/)

> **EtapKitap**, Pardus ETAP 23 ve okullardaki etkileşimli akıllı tahtalar için özel olarak tasarlanmış, C++20 ve Qt 6 tabanlı, yerel (native) ve çevrimdışı (offline) etkileşimli dijital ders kitabı motoru ve akıllı tahta çalışma ortamıdır.

Bu proje sıradan bir PDF okuyucu **değildir**. Standart PDF ders kitaplarını, zengin MEB/OGM formatında etkileşimli etkinliklerle (`.akbook` konteyneri), öğretmen sunum araçlarıyla ve çizim/yazı tahtasıyla tek bir bütünleşik dokunmatik arayüzde birleştirir.

---

## Temel Özellikler

### 1. Etkileşimli Ders Kitabı Motoru (`.akbook`)
- **Standart PDF + Akıllı Kitap Paketi**: Hem standart `.pdf` dosyalarını hem de `manifest.json`, medya ve etkinlik dosyalarını içeren `.akbook` paketlerini açar.
- **Normalize Koordinat Sistemi**: Sayfa çözünürlüğü ve yakınlaştırma (zoom) seviyesinden bağımsız `[0.0, 1.0]` ölçekli hotspot yerleşimi.
- **Etkileşim Tipleri**:
  - **Bilgi Kartı (`info`)**: Sayfa üzerine tıklandığında açılan kavram ve formül notları.
  - **Gömülü Ses (`audio`)**: Telaffuz ve dinleme metinleri.
  - **Video Oynatıcı (`video`)**: Deney ve problem çözüm videoları.
  - **İnteraktif Etkinlikler (`activity`)**: Çoktan seçmeli testler ve kavram eşleştirme modülleri.
- **Öğretmen Modu (Progressive Reveal)**: Sınıfta çözümü adım adım gösterme ve doğrudan doğru cevabı açma seçeneği.

### 2. Akıllı Tahta ve Sunum Araçları
- **Lazer İşaretçi (Laser Pointer)**: Ekranda dikkat çekmek için otomatik sönen parlak ışık izi.
- **Spot Işığı (Spotlight Tool)**: Ekranın geri kalanını karartıp sadece hedeflenen soru veya paragrafa odaklanan daire/dikdörtgen aydınlatma.
- **Perde (Curtain Tool)**: Sorunun adımlarını ve cevapları sırayla açmak için 4 yöne sürüklenebilir gizleme perdesi.
- **Ekran Dondurma (Freeze Canvas)**: Kitabın o anki durumunu dondurup üzerine karalama ve not alma.
- **Hızlı Yazı Tahtası (Quick Whiteboard)**: 6 farklı eğitim şablonu (Düz Beyaz, Kara Tahta Yeşili, Çizgili Defter, Kareli Grafik, Noktalı Izgara, Koordinat Sistemi).
- **Karalama Alanı (Scratchpad)**: Kitap sayfasının yanında açılan yan panel işlem alanı.

### 3. Çizim, Kalem ve Not Sistemi
- Yumuşatılmış vektör kalem çizgileri, fosforlu kalem (highlighter) modu ve renk paleti.
- Geometrik şekil araçları (Dikdörtgen, Elips, Çizgi, Ok).
- `QUndoStack` tabanlı sınırsız geri/ileri alma (Undo/Redo).
- Yerel SQLite veri tabanında (WAL modu) sayfa bazlı otomatik kayıt.

### 4. Güvenlik, Gizlilik ve Performans
- **%100 Çevrimdışı ve Yerel**: Telemetri, analitik veya dış ağ bağımlılığı bulunmaz.
- **Sıkı Paket Doğrulama**: ZIP path traversal (`../`) koruması, ZIP bombası önleme ve çalıştırılabilir dosya engelleme.
- **Çok İş parçacıklı PDF Motoru**: Sayfaların arka planda render edilmesi ve LRU bellek önbelleği.

---

## Proje Mimarisi

```text
EtapKitap/
├── src/
│   ├── core/         # Settings, Logging, ApplicationContext
│   ├── document/     # IDocument, PdfDocument, PageModel, Session
│   ├── pdf/          # QtPdfBackend, FallbackPdfBackend, RenderQueue, Cache
│   ├── book/         # BookManifest, ZipExtractor, SecurityValidator, BookPackage
│   ├── activities/   # IActivity, MultipleChoiceActivity, MatchingActivity
│   ├── annotations/  # StrokeAnnotation, ShapeAnnotation, UndoManager
│   ├── tools/        # LaserPointer, Spotlight, Curtain, Freeze, Whiteboard
│   ├── storage/      # DatabaseManager (SQLite WAL), Repositories, Autosave
│   ├── ui/           # ViewModels, Canvas Items, ImageProvider
│   └── ui/qml/       # Qt Quick Touch UI (HomeView, ReaderView, ToolDock, Modals)
├── demo/             # Örnek 'matematik9.akbook' paketi ve kaynak dosyaları
├── packaging/        # Pardus/Debian paketleme, .desktop, MIME x-akbook
├── tests/            # QtTest otomatik test paketi (5 test paketi)
└── docs/             # Detaylı mimari ve format dokümantasyonu
```

---

## Kurulum ve Derleme

### Sistem Gereksinimleri
- **İşletim Sistemi**: Pardus ETAP 23, Debian 12+, Ubuntu 22.04+ veya Windows 10/11
- **Derleyici**: C++20 destekli derleyici (GCC 11+, Clang 13+, MSVC 2022)
- **Framework**: Qt 6.4 veya üzeri (Qt Quick, QML, QuickControls2, Sql, Svg, Multimedia)

### Linux (Pardus ETAP / Debian / Ubuntu)

```bash
# Bağımlılıkları yükleyin
sudo apt-get update
sudo apt-get install -y build-essential cmake ninja-build pkg-config \
    qt6-base-dev qt6-declarative-dev qt6-multimedia-dev qt6-tools-dev \
    libqt6svg6-dev libqt6sql6-sqlite libsqlite3-dev zlib1g-dev libqt6pdf6-dev

# Derleyin
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DETAPKITAP_BUILD_TESTS=ON
cmake --build build --parallel

# Testleri çalıştırın
ctest --test-dir build --output-on-failure
```

### Windows (MSVC 2022)

```cmd
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DETAPKITAP_BUILD_TESTS=ON
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

---

## Hızlı Başlangıç

Derleme tamamlandıktan sonra örnek akıllı kitabı açabilirsiniz:

```bash
# Örnek etkileşimli kitabı başlat
./build/src/etapkitap demo/matematik9.akbook

# Veya herhangi bir standart PDF dosyasını aç
./build/src/etapkitap ders_notlari.pdf
```

---

## Pardus ETAP Paketleme

EtapKitap, Pardus ETAP için hazır Debian paket yapılandırması sunar:

```bash
# .deb paketi üret
dpkg-buildpackage -us -uc -b

# Paketi yükle
sudo dpkg -i ../etapkitap_1.0.0-1_amd64.deb
```

Sistem entegrasyonu:
- Uygulama Menüsü: `/usr/share/applications/etapkitap.desktop`
- MIME Tipi: `application/x-akbook` (`*.akbook` dosyaları çift tıklandığında otomatik açılır).

---

## Dokümantasyon

- [Mimari ve Tasarım Detayları](docs/ARCHITECTURE.md)
- [.akbook Paket Formatı ve Şeması](docs/BOOK_FORMAT.md)
- [Etkileşimli Etkinlikler ve Hotspotlar](docs/INTERACTIONS.md)
- [Derleme Kılavuzu](docs/BUILDING.md)
- [Paketleme ve Pardus Entegrasyonu](docs/PACKAGING.md)
- [Bakım ve Doğrulama Kanıtları](docs/oss-maintenance-evidence.md)

---

## Katkıda Bulunma ve Güvenlik

- Projeye katkı sağlamak için lütfen [CONTRIBUTING.md](CONTRIBUTING.md) belgesini inceleyiniz.
- Güvenlik bildirimleri ve güvenlik politikası için [SECURITY.md](SECURITY.md) belgesine başvurunuz.

---

## Lisans

EtapKitap, [Apache License 2.0](LICENSE) kapsamında açık kaynak olarak lisanslanmıştır.
Telif Hakkı (c) 2026 Tiyatrotist.

#include "app/AppController.h"
#include "core/ApplicationContext.h"
#include "core/Logger.h"
#include "activities/ActivityFactory.h"
#include "export/PdfExporter.h"
#include <QtCore/QFileInfo>

namespace EtapKitap {

AppController::AppController(QObject* parent)
    : QObject(parent)
    , m_docCtrl(std::make_unique<DocumentController>())
    , m_sessionState(std::make_unique<ReaderSessionState>())
    , m_annotationModel(std::make_unique<AnnotationModel>())
    , m_undoManager(std::make_unique<UndoManager>())
    , m_whiteboard(std::make_unique<WhiteboardEngine>())
    , m_scratchpad(std::make_unique<ScratchpadModel>())
    , m_freeze(std::make_unique<FreezeCanvas>())
    , m_searchEngine(std::make_unique<PdfSearchEngine>())
    , m_outlineModel(std::make_unique<PdfOutlineModel>())
    , m_interactionRegistry(std::make_unique<InteractionRegistry>())
{
    m_toolCtrl = std::make_unique<ToolController>(m_annotationModel.get(), m_undoManager.get(), this);
    m_activityModel = std::make_unique<ActivityViewModel>(this);

    auto* db = ApplicationContext::instance().databaseManager();
    m_sessionRepo = std::make_unique<SessionRepository>(db);
    m_annRepo = std::make_unique<AnnotationRepository>(db);
    m_autosaveWorker = std::make_unique<AutosaveWorker>(m_sessionRepo.get(), m_annRepo.get(), 1000, this);

    m_libraryModel = std::make_unique<LibraryViewModel>(m_sessionRepo.get(), this);

    setupAutosaveConnections();
}

AppController::~AppController() {
    if (m_autosaveWorker) {
        m_autosaveWorker->flushNow();
    }
}

DocumentController* AppController::docCtrl() { return m_docCtrl.get(); }
ReaderSessionState* AppController::sessionState() { return m_sessionState.get(); }
ToolController* AppController::toolCtrl() { return m_toolCtrl.get(); }
ActivityViewModel* AppController::activityModel() { return m_activityModel.get(); }
LibraryViewModel* AppController::libraryModel() { return m_libraryModel.get(); }
WhiteboardEngine* AppController::whiteboard() { return m_whiteboard.get(); }
ScratchpadModel* AppController::scratchpad() { return m_scratchpad.get(); }
FreezeCanvas* AppController::freeze() { return m_freeze.get(); }
PdfSearchEngine* AppController::searchEngine() { return m_searchEngine.get(); }
PdfOutlineModel* AppController::outlineModel() { return m_outlineModel.get(); }
Settings* AppController::settings() { return ApplicationContext::instance().settings(); }
AnnotationModel* AppController::annotationModel() { return m_annotationModel.get(); }
InteractionRegistry* AppController::interactionRegistry() { return m_interactionRegistry.get(); }

bool AppController::mediaModalOpen() const { return m_mediaModalOpen; }
void AppController::setMediaModalOpen(bool open) {
    if (m_mediaModalOpen != open) {
        m_mediaModalOpen = open;
        emit mediaModalOpenChanged(m_mediaModalOpen);
    }
}
QString AppController::mediaTitle() const { return m_mediaTitle; }
QString AppController::mediaSourceUrl() const { return m_mediaSourceUrl; }
bool AppController::isVideoMedia() const { return m_isVideoMedia; }

void AppController::setupAutosaveConnections() {
    connect(m_docCtrl.get(), &DocumentController::currentPageChanged, this, [this](int page) {
        if (!m_docCtrl->hasDocument()) return;
        DocumentSession s;
        s.documentId = m_docCtrl->currentDocument()->id();
        s.currentPage = page;
        s.zoomLevel = m_docCtrl->zoom();
        s.viewMode = m_docCtrl->viewMode();
        s.toolDockSide = m_sessionState->toolDockSide();
        m_autosaveWorker->triggerSessionSave(s);
    });

    connect(m_annotationModel.get(), &AnnotationModel::pageAnnotationsChanged, this, [this](int page) {
        if (!m_docCtrl->hasDocument()) return;
        QString docId = m_docCtrl->currentDocument()->id();
        auto anns = m_annotationModel->annotationsForPage(page);
        m_autosaveWorker->triggerPageAnnotationsSave(docId, page, anns);
        m_docCtrl->pageModel()->updateAnnotationsFlag(page, !anns.isEmpty());
    });
}

bool AppController::openDocument(const QString& filePath) {
    closeDocument();

    QFileInfo fi(filePath);
    if (!fi.exists()) {
        qCCritical(logCore) << "File does not exist:" << filePath;
        emit toastMessage(QStringLiteral("Dosya bulunamadı: ") + filePath);
        return false;
    }

    bool isAkbook = filePath.endsWith(QStringLiteral(".akbook"), Qt::CaseInsensitive) ||
                    (fi.isDir() && QFile::exists(QDir(filePath).filePath(QStringLiteral("manifest.json"))));

    if (isAkbook) {
        m_currentBookPackage = std::make_unique<BookPackage>();
        QString err;
        if (!m_currentBookPackage->open(filePath, &err)) {
            qCCritical(logBook) << "Failed to open book package:" << err;
            emit toastMessage(QStringLiteral("Kitap açılamadı: ") + err);
            m_currentBookPackage.reset();
            return false;
        }

        const auto& manifest = m_currentBookPackage->manifest();
        QString pdfPath = m_currentBookPackage->pdfDocumentPath();

        if (!m_docCtrl->openFile(pdfPath)) {
            emit toastMessage(QStringLiteral("Kitabın PDF içeriği yüklenemedi."));
            m_currentBookPackage.reset();
            return false;
        }

        // Load interactions & outline
        m_interactionRegistry->loadFromManifest(manifest);
        m_outlineModel->loadFromChapters(manifest.chapters);

        // Update page model interaction counts
        m_docCtrl->pageModel()->setDocument(m_docCtrl->currentDocument(), m_interactionRegistry->allPageInteractionCounts());

        // Record in SQLite
        m_sessionRepo->recordDocument(manifest.id, filePath, manifest.title, QStringLiteral("akbook"), m_docCtrl->pageCount());
        loadDocumentSessionAndAnnotations(manifest.id);

        emit bookLoaded();
        emit toastMessage(QStringLiteral("%1 açıldı — %2 sayfa").arg(manifest.title).arg(m_docCtrl->pageCount()));
        return true;
    } else {
        // Standard PDF
        if (!m_docCtrl->openFile(filePath)) {
            emit toastMessage(QStringLiteral("PDF dosyası açılamadı."));
            return false;
        }

        m_interactionRegistry->clear();
        m_outlineModel->clear();

        auto* doc = m_docCtrl->currentDocument();
        m_sessionRepo->recordDocument(doc->id(), filePath, doc->title(), QStringLiteral("pdf"), doc->pageCount());
        loadDocumentSessionAndAnnotations(doc->id());

        emit bookLoaded();
        emit toastMessage(QStringLiteral("%1 açıldı").arg(doc->title()));
        return true;
    }
}

void AppController::loadDocumentSessionAndAnnotations(const QString& docId) {
    // Restore session
    auto savedSession = m_sessionRepo->loadSession(docId);
    if (savedSession.has_value()) {
        m_docCtrl->setCurrentPage(savedSession->currentPage);
        m_docCtrl->setZoom(savedSession->zoomLevel);
        m_docCtrl->setViewMode(savedSession->viewMode);
        m_sessionState->setToolDockSide(savedSession->toolDockSide);
    }

    // Restore annotations
    auto allAnns = m_annRepo->loadAllDocumentAnnotations(docId);
    m_annotationModel->loadAnnotations(allAnns);

    // Update annotation flags in page model
    for (int p = 1; p <= m_docCtrl->pageCount(); ++p) {
        m_docCtrl->pageModel()->updateAnnotationsFlag(p, m_annotationModel->countForPage(p) > 0);
    }
}

void AppController::closeDocument() {
    if (m_autosaveWorker) {
        m_autosaveWorker->flushNow();
    }
    m_docCtrl->closeDocument();
    m_annotationModel->clearAll();
    m_undoManager->clear();
    m_interactionRegistry->clear();
    m_outlineModel->clear();
    m_searchEngine->clear();
    m_currentBookPackage.reset();
    setMediaModalOpen(false);
    m_activityModel->close();
}

void AppController::triggerHotspot(const QString& interactionId) {
    const auto* hi = m_interactionRegistry->findById(interactionId);
    if (!hi) {
        qCWarning(logBook) << "Hotspot not found:" << interactionId;
        return;
    }

    qCInfo(logBook) << "Triggered hotspot:" << hi->id << "Type:" << hi->typeString();

    switch (hi->type) {
    case InteractionType::Video:
    case InteractionType::Audio: {
        if (m_currentBookPackage) {
            QUrl url = m_currentBookPackage->resolveAssetUrl(hi->resource);
            m_mediaTitle = hi->title;
            m_mediaSourceUrl = url.toString();
            m_isVideoMedia = (hi->type == InteractionType::Video);
            emit mediaModalChanged();
            setMediaModalOpen(true);
        }
        break;
    }

    case InteractionType::Activity: {
        if (m_currentBookPackage && !hi->activityFile.isEmpty()) {
            QByteArray actData = m_currentBookPackage->readAssetData(hi->activityFile);
            QString err;
            auto act = ActivityFactory::createFromJson(actData, &err);
            if (act) {
                m_activityModel->setActivity(std::move(act));
            } else {
                emit toastMessage(QStringLiteral("Etkinlik yüklenemedi: ") + err);
            }
        }
        break;
    }

    case InteractionType::InfoCard:
    case InteractionType::TeacherHint:
    case InteractionType::Solution: {
        emit toastMessage(hi->title + QStringLiteral(": ") + hi->teaser);
        break;
    }

    default:
        break;
    }
}

bool AppController::exportAnnotatedPdf(const QString& outputPath) {
    if (!m_docCtrl->hasDocument()) return false;
    QString err;
    bool ok = PdfExporter::exportAnnotatedPdf(m_docCtrl->currentDocument(), m_annRepo.get(), outputPath, &err);
    if (ok) {
        emit toastMessage(QStringLiteral("PDF başarıyla dışa aktarıldı: ") + outputPath);
    } else {
        emit toastMessage(QStringLiteral("Dışa aktarma hatası: ") + err);
    }
    return ok;
}

void AppController::addCurrentPageBookmark(const QString& title) {
    if (!m_docCtrl->hasDocument()) return;
    int page = m_docCtrl->currentPage();
    m_sessionRepo->addBookmark(m_docCtrl->currentDocument()->id(), page, title);
    emit toastMessage(QStringLiteral("Sayfa %1 yer imlerine eklendi").arg(page));
}

void AppController::removeBookmark(const QString& bookmarkId) {
    m_sessionRepo->removeBookmark(bookmarkId);
}

void AppController::freezeScreen(const QImage& snapshot) {
    m_freeze->freeze(snapshot);
    m_sessionState->setFreezeOpen(true);
}

} // namespace EtapKitap

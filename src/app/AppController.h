#pragma once

#include "document/DocumentController.h"
#include "book/InteractiveBook.h"
#include "book/BookPackage.h"
#include "book/InteractionRegistry.h"
#include "annotations/AnnotationModel.h"
#include "annotations/UndoManager.h"
#include "ui/viewmodels/ReaderSessionState.h"
#include "ui/viewmodels/ToolController.h"
#include "ui/viewmodels/ActivityViewModel.h"
#include "ui/viewmodels/LibraryViewModel.h"
#include "tools/WhiteboardEngine.h"
#include "tools/ScratchpadModel.h"
#include "tools/FreezeCanvas.h"
#include "pdf/PdfSearchEngine.h"
#include "pdf/PdfOutlineModel.h"
#include "storage/SessionRepository.h"
#include "storage/AnnotationRepository.h"
#include "storage/AutosaveWorker.h"
#include "core/Settings.h"
#include <QtCore/QObject>
#include <memory>

namespace EtapKitap {

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(DocumentController* docCtrl READ docCtrl CONSTANT)
    Q_PROPERTY(ReaderSessionState* sessionState READ sessionState CONSTANT)
    Q_PROPERTY(ToolController* toolCtrl READ toolCtrl CONSTANT)
    Q_PROPERTY(ActivityViewModel* activityModel READ activityModel CONSTANT)
    Q_PROPERTY(LibraryViewModel* libraryModel READ libraryModel CONSTANT)
    Q_PROPERTY(WhiteboardEngine* whiteboard READ whiteboard CONSTANT)
    Q_PROPERTY(ScratchpadModel* scratchpad READ scratchpad CONSTANT)
    Q_PROPERTY(FreezeCanvas* freeze READ freeze CONSTANT)
    Q_PROPERTY(PdfSearchEngine* searchEngine READ searchEngine CONSTANT)
    Q_PROPERTY(PdfOutlineModel* outlineModel READ outlineModel CONSTANT)
    Q_PROPERTY(Settings* settings READ settings CONSTANT)
    Q_PROPERTY(AnnotationModel* annotationModel READ annotationModel CONSTANT)
    Q_PROPERTY(InteractionRegistry* interactionRegistry READ interactionRegistry NOTIFY bookLoaded)

    // Active media modal properties
    Q_PROPERTY(bool mediaModalOpen READ mediaModalOpen WRITE setMediaModalOpen NOTIFY mediaModalOpenChanged)
    Q_PROPERTY(QString mediaTitle READ mediaTitle NOTIFY mediaModalChanged)
    Q_PROPERTY(QString mediaSourceUrl READ mediaSourceUrl NOTIFY mediaModalChanged)
    Q_PROPERTY(bool isVideoMedia READ isVideoMedia NOTIFY mediaModalChanged)

public:
    explicit AppController(QObject* parent = nullptr);
    ~AppController() override;

    [[nodiscard]] DocumentController* docCtrl();
    [[nodiscard]] ReaderSessionState* sessionState();
    [[nodiscard]] ToolController* toolCtrl();
    [[nodiscard]] ActivityViewModel* activityModel();
    [[nodiscard]] LibraryViewModel* libraryModel();
    [[nodiscard]] WhiteboardEngine* whiteboard();
    [[nodiscard]] ScratchpadModel* scratchpad();
    [[nodiscard]] FreezeCanvas* freeze();
    [[nodiscard]] PdfSearchEngine* searchEngine();
    [[nodiscard]] PdfOutlineModel* outlineModel();
    [[nodiscard]] Settings* settings();
    [[nodiscard]] AnnotationModel* annotationModel();
    [[nodiscard]] InteractionRegistry* interactionRegistry();

    [[nodiscard]] bool mediaModalOpen() const;
    void setMediaModalOpen(bool open);
    [[nodiscard]] QString mediaTitle() const;
    [[nodiscard]] QString mediaSourceUrl() const;
    [[nodiscard]] bool isVideoMedia() const;

    Q_INVOKABLE bool openDocument(const QString& filePath);
    Q_INVOKABLE void closeDocument();
    Q_INVOKABLE void triggerHotspot(const QString& interactionId);
    Q_INVOKABLE bool exportAnnotatedPdf(const QString& outputPath);
    Q_INVOKABLE void addCurrentPageBookmark(const QString& title = QString());
    Q_INVOKABLE void removeBookmark(const QString& bookmarkId);
    Q_INVOKABLE void freezeScreen(const QImage& snapshot);

signals:
    void bookLoaded();
    void mediaModalOpenChanged(bool open);
    void mediaModalChanged();
    void toastMessage(const QString& message);

private:
    void setupAutosaveConnections();
    void loadDocumentSessionAndAnnotations(const QString& docId);

    std::unique_ptr<DocumentController> m_docCtrl;
    std::unique_ptr<ReaderSessionState> m_sessionState;
    std::unique_ptr<AnnotationModel> m_annotationModel;
    std::unique_ptr<UndoManager> m_undoManager;
    std::unique_ptr<ToolController> m_toolCtrl;
    std::unique_ptr<ActivityViewModel> m_activityModel;
    std::unique_ptr<LibraryViewModel> m_libraryModel;
    std::unique_ptr<WhiteboardEngine> m_whiteboard;
    std::unique_ptr<ScratchpadModel> m_scratchpad;
    std::unique_ptr<FreezeCanvas> m_freeze;
    std::unique_ptr<PdfSearchEngine> m_searchEngine;
    std::unique_ptr<PdfOutlineModel> m_outlineModel;

    std::unique_ptr<SessionRepository> m_sessionRepo;
    std::unique_ptr<AnnotationRepository> m_annRepo;
    std::unique_ptr<AutosaveWorker> m_autosaveWorker;

    std::unique_ptr<BookPackage> m_currentBookPackage;
    std::unique_ptr<InteractionRegistry> m_interactionRegistry;

    bool m_mediaModalOpen{false};
    QString m_mediaTitle;
    QString m_mediaSourceUrl;
    bool m_isVideoMedia{true};
};

} // namespace EtapKitap

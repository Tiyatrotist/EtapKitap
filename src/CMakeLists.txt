set(CORE_SOURCES
    core/Logger.cpp
    core/Settings.cpp
    core/ApplicationContext.cpp

    document/DocumentController.cpp
    document/PageModel.cpp
    document/PdfDocument.cpp

    pdf/FallbackPdfBackend.cpp
    pdf/QtPdfBackend.cpp
    pdf/PdfPageCache.cpp
    pdf/PdfRenderQueue.cpp
    pdf/PdfOutlineModel.cpp
    pdf/PdfSearchEngine.cpp

    book/BookManifest.cpp
    book/BookSecurityValidator.cpp
    book/ZipExtractor.cpp
    book/BookPackage.cpp
    book/InteractionRegistry.cpp
    book/InteractiveBook.cpp

    activities/MultipleChoiceActivity.cpp
    activities/MatchingActivity.cpp
    activities/ActivityFactory.cpp

    annotations/StrokeAnnotation.cpp
    annotations/ShapeAnnotation.cpp
    annotations/AnnotationCommands.cpp
    annotations/UndoManager.cpp
    annotations/AnnotationModel.cpp

    tools/LaserPointer.cpp
    tools/SpotlightTool.cpp
    tools/CurtainTool.cpp
    tools/FreezeCanvas.cpp
    tools/WhiteboardEngine.cpp
    tools/ScratchpadModel.cpp

    storage/Migrations.cpp
    storage/DatabaseManager.cpp
    storage/SessionRepository.cpp
    storage/AnnotationRepository.cpp
    storage/AutosaveWorker.cpp

    export/PdfExporter.cpp
    export/ScreenCapture.cpp

    ui/viewmodels/ReaderSessionState.cpp
    ui/viewmodels/ToolController.cpp
    ui/viewmodels/ActivityViewModel.cpp
    ui/viewmodels/LibraryViewModel.cpp
    ui/DrawingCanvasItem.cpp
    ui/WhiteboardCanvasItem.cpp
    ui/PageImageProvider.cpp

    app/AppController.cpp
)

set(CORE_HEADERS
    core/Types.h
    core/Logger.h
    core/Settings.h
    core/ApplicationContext.h

    document/IDocument.h
    document/DocumentSession.h
    document/DocumentController.h
    document/PageModel.h
    document/PdfDocument.h

    pdf/IPdfBackend.h
    pdf/FallbackPdfBackend.h
    pdf/QtPdfBackend.h
    pdf/PdfPageCache.h
    pdf/PdfRenderQueue.h
    pdf/PdfOutlineModel.h
    pdf/PdfSearchEngine.h

    book/BookManifest.h
    book/BookSecurityValidator.h
    book/ZipExtractor.h
    book/BookPackage.h
    book/InteractionRegistry.h
    book/InteractiveBook.h

    activities/IActivity.h
    activities/MultipleChoiceActivity.h
    activities/MatchingActivity.h
    activities/ActivityFactory.h

    annotations/Annotation.h
    annotations/StrokeAnnotation.h
    annotations/ShapeAnnotation.h
    annotations/AnnotationCommands.h
    annotations/UndoManager.h
    annotations/AnnotationModel.h

    tools/LaserPointer.h
    tools/SpotlightTool.h
    tools/CurtainTool.h
    tools/FreezeCanvas.h
    tools/WhiteboardEngine.h
    tools/ScratchpadModel.h

    storage/Migrations.h
    storage/DatabaseManager.h
    storage/SessionRepository.h
    storage/AnnotationRepository.h
    storage/AutosaveWorker.h

    export/PdfExporter.h
    export/ScreenCapture.h

    ui/viewmodels/ReaderSessionState.h
    ui/viewmodels/ToolController.h
    ui/viewmodels/ActivityViewModel.h
    ui/viewmodels/LibraryViewModel.h
    ui/DrawingCanvasItem.h
    ui/WhiteboardCanvasItem.h
    ui/PageImageProvider.h

    app/AppController.h
)

add_library(etapkitap_core STATIC
    ${CORE_SOURCES}
    ${CORE_HEADERS}
)

target_include_directories(etapkitap_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(etapkitap_core PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
    Qt6::QuickControls2
    Qt6::Sql
    Qt6::Svg
    Qt6::Concurrent
    ZLIB::ZLIB
)

if(TARGET Qt6::Pdf)
    target_link_libraries(etapkitap_core PUBLIC Qt6::Pdf)
    target_compile_definitions(etapkitap_core PUBLIC ETAPKITAP_HAS_QTPDF)
endif()

if(TARGET Qt6::Multimedia)
    target_link_libraries(etapkitap_core PUBLIC Qt6::Multimedia)
    target_compile_definitions(etapkitap_core PUBLIC ETAPKITAP_HAS_MULTIMEDIA)
endif()

# Main executable
qt_add_executable(etapkitap
    main.cpp
)

target_link_libraries(etapkitap PRIVATE
    etapkitap_core
)

qt_add_qml_module(etapkitap
    URI EtapKitap
    VERSION 1.0
    RESOURCE_PREFIX "/"
    QML_FILES
        ui/qml/main.qml
        ui/qml/components/EtapTheme.qml
        ui/qml/components/IconButton.qml
        ui/qml/components/ToastNotification.qml
        ui/qml/panels/TopBar.qml
        ui/qml/panels/NavigationDrawer.qml
        ui/qml/panels/ToolDock.qml
        ui/qml/panels/ToolPropertyPopover.qml
        ui/qml/panels/BottomPageBar.qml
        ui/qml/overlays/HotspotOverlay.qml
        ui/qml/overlays/DrawingCanvas.qml
        ui/qml/overlays/LaserOverlay.qml
        ui/qml/overlays/SpotlightOverlay.qml
        ui/qml/overlays/CurtainOverlay.qml
        ui/qml/overlays/FreezeOverlay.qml
        ui/qml/dialogs/MediaModal.qml
        ui/qml/dialogs/ActivityModal.qml
        ui/qml/dialogs/WhiteboardModal.qml
        ui/qml/dialogs/ScratchpadPane.qml
        ui/qml/views/HomeView.qml
        ui/qml/views/ReaderView.qml
)

install(TARGETS etapkitap
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
)

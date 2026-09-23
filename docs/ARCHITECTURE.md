# EtapKitap Architecture

This document describes the high-level architecture, module decomposition, and data flow of **EtapKitap**.

---

## 1. Architectural Overview

EtapKitap combines an interactive digital textbook engine with an educational smartboard presentation workspace into a single native C++20 and Qt 6 application.

```
+-------------------------------------------------------------------------------+
|                             Qt Quick / QML Layer                             |
|  [HomeView] [ReaderView] [ToolDock] [TopBar] [Overlays: Laser, Spot, Curtain] |
|  [Modals: MediaModal, ActivityModal, WhiteboardModal, ScratchpadPane]         |
+---------------------------------------+---------------------------------------+
                                        | Q_PROPERTY / Invokable / Models
+---------------------------------------v---------------------------------------+
|                                ViewModels & UI C++                            |
|  - AppController         - ReaderSessionState       - ToolController          |
|  - ActivityViewModel     - LibraryViewModel         - DrawingCanvasItem       |
|  - WhiteboardCanvasItem  - PageImageProvider                                  |
+---------------------------------------+---------------------------------------+
                                        | Domain Calls
+---------------------+-----------------+------------------+--------------------+
|  Document & PDF     | Interactive Book| Annotation System| Tools & Whiteboard |
|  - IDocument        | - BookPackage   | - AnnotationModel| - LaserPointer     |
|  - PdfDocument      | - BookManifest  | - StrokeAnnot    | - SpotlightTool    |
|  - IPdfBackend      | - Interactions  | - ShapeAnnot     | - CurtainTool      |
|  - QtPdfBackend     | - ActivityEngine| - UndoManager    | - FreezeCanvas     |
|  - PdfPageCache     | - ZipExtractor  | - Commands       | - WhiteboardEngine |
|  - PdfRenderQueue   | - SecurityValid |                  | - ScratchpadModel  |
+---------------------+-----------------+------------------+--------------------+
                                        | Persistence / Export
+---------------------------------------v---------------------------------------+
|                         Storage & Core Infrastructure                         |
|  - DatabaseManager (SQLite WAL)         - SessionRepository                   |
|  - AnnotationRepository                 - AutosaveWorker                      |
|  - ApplicationContext                   - Settings / Logger                   |
+-------------------------------------------------------------------------------+
```

---

## 2. Core Subsystems

### 2.1 Document & PDF Engine (`src/pdf/`, `src/document/`)
- **Backend Abstraction**: `IPdfBackend` defines the rendering interface.
  - `QtPdfBackend`: Uses `QPdfDocument` when Qt6 PDF is available.
  - `FallbackPdfBackend`: A procedural vector renderer ensuring graceful operation even on lean environments without QtPdf.
- **Asynchronous Rendering Queue**: `PdfRenderQueue` runs background render jobs across CPU worker threads via `QThreadPool`, avoiding main UI thread lockups.
- **LRU Memory Cache**: `PdfPageCache` maintains rendered raster pages with configurable memory limits (default 256MB), proactively evicting distant pages.

### 2.2 Book Package & Interaction Engine (`src/book/`, `src/activities/`)
- **Package Container (`.akbook`)**: Extensible ZIP archive containing `manifest.json`, `document.pdf`, assets, and structured activity JSON files.
- **Security Boundaries**: `BookSecurityValidator` rejects malicious archives (path traversal, zip bombs, excessive compression ratios, executable payload types).
- **Normalized Coordinates**: All hotspots and overlay interactions are positioned via `[0.0, 1.0]` normalized page coordinates (`NormalizedRect`), ensuring precise alignment across all screen resolutions and zoom levels.
- **Activity Engine**: Modular `IActivity` interface parsed by `ActivityFactory` supporting:
  - `MultipleChoiceActivity`: Single/multi-choice questions, shuffle options, instant feedback, teacher answer reveal.
  - `MatchingActivity`: Interactive pair matching with validation.

### 2.3 Presentation & Classroom Tools (`src/tools/`)
- **LaserPointer**: Emits a decaying luminosity trail with configurable duration and radius.
- **SpotlightTool**: Creates an illuminated circular or rectangular viewing aperture over dimmed background.
- **CurtainTool**: 4-direction draggable privacy shade for step-by-step classroom problem solving.
- **FreezeCanvas**: Instant raster capture allowing teachers to write over a frozen frame without altering the underlying document.
- **WhiteboardEngine**: Full-screen digital whiteboard supporting 6 educational templates (Blank, Chalkboard, Lined, Grid, Dotted, Coordinate System).
- **ScratchpadModel**: Side-by-side scratchpad for rapid on-screen calculations.

### 2.4 Annotation & Undo Stack (`src/annotations/`)
- **Vector Strokes & Geometric Shapes**: Freehand smoothing (`QPainterPath`), variable thickness, and shapes (Rectangle, Ellipse, Line, Arrow).
- **Command Pattern**: All drawing operations are encapsulated in `QUndoCommand` subclasses managed by `UndoManager`.
- **Layered Painting**: QML `DrawingCanvasItem` renders vector paths natively via `QPainter` with anti-aliasing and layer caching.

### 2.5 Storage & Persistence (`src/storage/`)
- **Local SQLite DB**: Uses SQLite with `WAL` (Write-Ahead Logging) and `synchronous = NORMAL` for robust crash resilience.
- **Schema Migrations**: Linear migrations manage table creation and upgrades automatically.
- **Debounced Autosave**: `AutosaveWorker` batches unsaved strokes and session states (last read page, zoom level) without impacting interactive touch responsiveness.

---

## 3. Threading & Concurrency Model

1. **Main UI Thread**: Handles Qt Quick scene graph, touch input gestures, QML state machine, and canvas paint requests.
2. **PDF Render Workers (`QThreadPool`)**: High-resolution page rasterization occurs off the UI thread and dispatches signals on completion.
3. **Autosave Worker**: Asynchronously commits dirty annotation batches to SQLite on a background timer.

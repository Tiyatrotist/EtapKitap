# Changelog

All notable changes to **EtapKitap** will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [1.0.0] - 2026-09-23

### Added
- **Interactive Textbook Engine**:
  - Unified runtime for standard `.pdf` and extensible `.akbook` package containers.
  - Normalized coordinate system (0.0 to 1.0) ensuring resolution- and zoom-independent hotspot and annotation placement.
  - Hotspot types: Information card (`info`), Embedded Audio (`audio`), Video player (`video`), and Interactive Activities (`activity`).
  - Interactive activity engine supporting Multiple Choice quizzes and Matching pairs with immediate evaluation and teacher progressive reveal.
- **Smartboard & Presentation Workspace**:
  - Teacher presentation tools: Laser pointer with glowing auto-fading trail, Spotlight tool with circular/rectangular dimming mask, Curtain shade tool with 4-direction dragging, Freeze mode screen capture canvas.
  - Quick Whiteboard workspace with 6 background templates (blank white, chalkboard green/black, lined notebook, grid graph, dotted grid, coordinate axes).
  - Split-pane Scratchpad for scratch calculations alongside open textbook pages.
  - Pen & shape annotation overlay with smoothing, pressure simulation, highlighter mode, geometric shape drawing (rectangles, ellipses, arrows, lines), and undo/redo command stack.
- **High-Performance PDF Engine**:
  - Dual backend architecture: Native `QtPdf` integration with fallback vector procedural renderer.
  - Multi-threaded asynchronous rendering pool with LRU page cache and memory budgeting.
- **Storage & Crash Recovery**:
  - Local-first SQLite database with WAL mode and schema migrations.
  - Automatic debounced session and annotation autosave.
  - Per-book and per-page persistent annotation storage.
- **Pardus ETAP Desktop Integration**:
  - Standard XDG Desktop entry (`etapkitap.desktop`), MIME type registration (`application/x-akbook`), and AppStream metadata (`etapkitap.metainfo.xml`).
  - Debian packaging rules and control files for ETAP 23.
- **Automated Verification**:
  - Test suites for manifest parsing, zip extraction security, stroke & shape annotations, SQLite storage/migrations, and activity validation (5/5 tests passing).

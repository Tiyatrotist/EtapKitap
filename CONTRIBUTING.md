# Contributing to EtapKitap

Thank you for your interest in contributing to **EtapKitap**! EtapKitap is an open-source interactive digital textbook runtime and smartboard workspace developed for Pardus ETAP 23 and classroom touchscreens.

We welcome contributions from educators, developers, and packaging maintainers.

---

## Code of Conduct & Principles

1. **Classroom-First & Student Privacy**: EtapKitap operates strictly local-first and offline-first. No telemetry, user tracking, or third-party cloud dependencies may be introduced.
2. **Deterministic & Native**: EtapKitap is built using native C++20 and Qt 6. No heavyweight web wrappers (Electron, Node.js) or Python runtime dependencies are permitted.
3. **Pardus ETAP Standards**: Integrations must adhere to standard Linux desktop specifications (`.desktop`, XDG MIME, AppStream metainfo) without modifying core system files.

---

## Development Setup

### Prerequisites

- **Linux (Pardus / Debian / Ubuntu)**:
  ```bash
  sudo apt-get update
  sudo apt-get install -y build-essential cmake ninja-build pkg-config \
      qt6-base-dev qt6-declarative-dev qt6-multimedia-dev qt6-tools-dev \
      libqt6svg6-dev libqt6sql6-sqlite libsqlite3-dev zlib1g-dev libqt6pdf6-dev
  ```

- **Windows**:
  - Visual Studio 2022 (MSVC C++20)
  - Qt 6.4+ (msvc2022_64)
  - CMake 3.22+ and Ninja

### Building from Source

```bash
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DETAPKITAP_BUILD_TESTS=ON
cmake --build build --parallel
```

### Running Tests

EtapKitap enforces automated test verification:

```bash
ctest --test-dir build --output-on-failure
```

---

## Contribution Guidelines

### Workflow

1. **Fork the repository** on GitHub.
2. **Create a topic branch**: `git checkout -b feature/my-feature` or `git checkout -b fix/my-fix`.
3. **Commit your changes**: Ensure clean commit messages explaining *why* the change is made.
   - Note: Do not include personal local usernames, emails, or absolute machine paths in commit metadata or documentation.
4. **Run the test suite**: All 5 test targets (`test_manifest`, `test_security`, `test_annotations`, `test_storage`, `test_activities`) must pass without errors.
5. **Open a Pull Request**: Provide a clear explanation in the pull request template.

### Code Style

- **Standard**: C++20, clean modern idioms (RAII, smart pointers, `[[nodiscard]]`, const-correctness).
- **Qt Guidelines**: Use `QStringLiteral`, `Q_OBJECT`, signals/slots, and declarative QML separation where appropriate.
- **Error Handling**: Graceful degradation (e.g., fallback rendering when QtPdf is absent, strict validation for package extraction).
- **Security**: All file parsing, ZIP extraction, and JSON reading must enforce boundaries (path traversal guards, zip bomb mitigations, format version checks).

---

## Good First Issues

If you are looking for introductory areas to contribute:
- Additional QML activity templates (e.g., fill-in-the-blanks, flashcards).
- Additional whiteboard background grid templates.
- Localization (adding translations in `assets/translations/`).
- Documentation enhancements for teachers creating `.akbook` packages.

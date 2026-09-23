# OSS Maintenance & Verification Evidence

This document records factual maintenance activity, test execution results, CI configuration, and architecture readiness for **EtapKitap**.

---

## 1. Project Identity & Governance

- **Project Name**: EtapKitap
- **Public Maintainer Identity**: Tiyatrotist (`179411334+Tiyatrotist@users.noreply.github.com`)
- **License**: Apache License 2.0
- **Supported OS Platforms**: Pardus ETAP 23 (Debian-based), Linux desktop environments (X11 / Wayland), Windows 10/11
- **Architecture**: Native C++20 with Qt 6 (Qt Quick / QML)

---

## 2. CI and Automated Verification

- **Continuous Integration Workflow**: `.github/workflows/ci.yml`
  - Platform: Ubuntu Linux x86_64
  - Pipeline: Clean build with CMake 3.22+, Ninja, Qt 6, and execution of automated test suites under CTest.
- **Local Test Suite Run (Recorded 2026-09-23)**:
  ```text
  Test project: build
      Start 1: test_manifest
  1/5 Test #1: test_manifest ....................   Passed    0.22 sec
      Start 2: test_security
  2/5 Test #2: test_security ....................   Passed    0.16 sec
      Start 3: test_annotations
  3/5 Test #3: test_annotations .................   Passed    0.20 sec
      Start 4: test_storage
  4/5 Test #4: test_storage .....................   Passed    0.21 sec
      Start 5: test_activities
  5/5 Test #5: test_activities ..................   Passed    0.18 sec

  100% tests passed out of 5
  Total Test time (real) = 0.98 sec
  ```

---

## 3. Verified Security & Privacy Boundaries

- **Zero Network Ingestion**: No outbound HTTP/HTTPS requests, telemetry, or remote crash reporters.
- **Archive Extraction Bounds**: Safe decompression with path traversal defense, file size limits (100MB), compression ratio limits (100:1), and package limits (500MB).
- **Offline Storage**: Local SQLite with Write-Ahead Logging (WAL) and automatic migration management.

---

## 4. Release History & Milestones

- **v1.0.0 (2026-09-23)**:
  - Initial public release of EtapKitap runtime.
  - Interactive `.akbook` package support.
  - Teacher presentation tools (Laser, Spotlight, Curtain, Freeze Canvas).
  - Quick Whiteboard (6 background templates) and Scratchpad.
  - Pardus ETAP 23 packaging integration (`.desktop`, MIME `application/x-akbook`, AppStream).

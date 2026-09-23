# Security Policy

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 1.0.x   | :white_check_mark: |
| < 1.0   | :x:                |

---

## Security Boundaries & Design Principles

EtapKitap is engineered with strict boundaries suited for public school classroom environments:

1. **Local-First & Zero Telemetry**:
   - EtapKitap does not connect to remote servers or analytics backends.
   - All persistence is maintained locally in an encrypted/sanitized SQLite database under the user's standard application data directory.

2. **Package Security (`.akbook` Containers)**:
   - All ZIP file entries are sanitized prior to extraction via `BookSecurityValidator`.
   - **Path Traversal Guard**: Filenames containing `..`, absolute paths, drive letters, or null bytes are rejected immediately.
   - **Zip Bomb & Decompression Limits**: Single-file uncompressed limits (100 MB max), compression ratio thresholds (100:1 max), and overall package uncompressed caps (500 MB max) are strictly enforced.
   - **Executable Extension Blacklist**: Executable binary extensions (`.exe`, `.sh`, `.bin`, `.so`, `.appimage`, `.bat`, etc.) inside packages are blocked.

3. **Database Security**:
   - SQLite uses parameterized queries (`QSqlQuery::prepare` and `addBindValue`), preventing SQL injection attacks.
   - WAL (Write-Ahead Logging) mode is activated with synchronous NORMAL to guard against crash-induced corruption.

---

## Reporting a Vulnerability

If you discover a security vulnerability in EtapKitap:

1. **Do not open a public issue.**
2. Report the vulnerability privately via GitHub Security Advisories or by emailing the maintainer:
   - Maintainer: **Tiyatrotist** (`179411334+Tiyatrotist@users.noreply.github.com`).
3. Include detailed steps to reproduce the issue, along with sample payload files if applicable.
4. You will receive an acknowledgment within 48 hours, followed by updates on remediation.

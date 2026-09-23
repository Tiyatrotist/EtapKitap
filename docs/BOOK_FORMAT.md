# The .akbook Package Format Specification

The `.akbook` (Akıllı Kitap / Smart Book) format is an open, archive-based container format for interactive textbooks designed for classroom smartboard software.

---

## 1. Archive Structure

An `.akbook` file is a standard ZIP archive with a defined internal layout:

```text
my_book.akbook
├── manifest.json                  # Required: package metadata & hotspot definitions
├── document.pdf                   # Required: base PDF textbook content
├── assets/                        # Optional: media assets
│   ├── audio/                     # Audio explanations (.mp3, .ogg, .wav)
│   ├── video/                     # Demonstration videos (.mp4, .webm)
│   └── images/                    # High-res diagrams (.png, .jpg, .svg)
└── activities/                    # Optional: interactive exercises
    ├── quizzes/                   # Multiple choice activities (.json)
    └── matching/                  # Concept matching activities (.json)
```

---

## 2. Manifest Schema (`manifest.json`)

The manifest is formatted in UTF-8 JSON.

```json
{
  "$schema": "https://raw.githubusercontent.com/Tiyatrotist/EtapKitap/main/schemas/manifest.schema.json",
  "formatVersion": 1,
  "id": "matematik-9-demo",
  "title": "Matematik 9 — Etkileşimli Ders Kitabı",
  "author": "MEB / Tiyatrotist",
  "language": "tr",
  "document": "document.pdf",
  "chapters": [
    { "title": "Ünite 1: Mantık", "page": 1 },
    { "title": "Ünite 2: Kümeler", "page": 15 }
  ],
  "pages": [
    {
      "page": 2,
      "interactions": [
        {
          "id": "info-tanim-1",
          "type": "info",
          "title": "Önerme Tanımı",
          "teaser": "Doğru ya da yanlış kesin bir hüküm bildiren ifadelere önerme denir.",
          "rect": { "x": 0.82, "y": 0.22, "width": 0.08, "height": 0.08 }
        },
        {
          "id": "quiz-soru-1",
          "type": "activity",
          "title": "Kavrama Testi",
          "activityFile": "activities/quizzes/q1.json",
          "rect": { "x": 0.82, "y": 0.45, "width": 0.08, "height": 0.08 }
        }
      ]
    }
  ]
}
```

### Manifest Fields

| Field | Type | Required | Description |
|---|---|---|---|
| `formatVersion` | integer | Yes | Package specification version (currently `1`). |
| `id` | string | Yes | Unique identifier (slug: lowercase, hyphens). |
| `title` | string | Yes | Human-readable book title. |
| `document` | string | Yes | Relative path to the underlying PDF (`document.pdf`). |
| `language` | string | No | Two-letter ISO language code (e.g. `tr`, `en`). |
| `chapters` | array | No | List of chapter objects `{ "title": string, "page": integer }`. |
| `pages` | array | No | List of page interaction definitions. |

---

## 3. Normalized Coordinates

All hotspot bounding boxes are defined using normalized coordinates in the range `[0.0, 1.0]`:

- `x`: Horizontal distance from the left edge of the page divided by page width.
- `y`: Vertical distance from the top edge of the page divided by page height.
- `width`: Width of the hotspot box divided by page width.
- `height`: Height of the hotspot box divided by page height.

```text
(0,0) -------------------------------+
|                                    |
|       (x, y)                       |
|         +---------+                |
|         | Hotspot | height         |
|         +---------+                |
|            width                   |
|                                    |
+------------------------------- (1,1)
```

**Advantage**: Because coordinates are normalized, hotspots remain in exact alignment regardless of display DPI, 4K scaling, aspect ratio, or reader zoom factors.

---

## 4. Packaging and Security Verification

To create an `.akbook` package manually:

```bash
cd my_book_folder/
zip -r ../my_book.akbook manifest.json document.pdf assets/ activities/
```

Security guidelines enforced on extraction:
1. No path traversal elements (`..` or root `/`).
2. Maximum individual file size: 100 MB.
3. Maximum uncompressed total size: 500 MB.
4. Maximum compression ratio: 100:1.
5. Prohibited file types: `.exe`, `.dll`, `.so`, `.sh`, `.bin`, `.appimage`, `.bat`.

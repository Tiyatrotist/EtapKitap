# Interactive Activities and Hotspots

EtapKitap embeds interactive teaching widgets and hotspots directly onto textbook pages without modifying the original PDF layout.

---

## 1. Hotspot Types

Hotspots are declared in `manifest.json` under `pages[].interactions[]`.

### 1.1 Information Card (`info`)
Displays an expandable pedagogical note, hint, or formula summary in an overlay modal.

```json
{
  "id": "info-1",
  "type": "info",
  "title": "Kritik Tanım",
  "teaser": "Önerme; doğru ya da yanlış kesin bir hüküm bildiren ifadedir.",
  "rect": { "x": 0.85, "y": 0.20, "width": 0.08, "height": 0.08 }
}
```

### 1.2 Embedded Audio (`audio`)
Plays an audio narration, pronunciation sample, or listening comprehension track.

```json
{
  "id": "audio-1",
  "type": "audio",
  "title": "Dinleme Metni",
  "resource": "assets/audio/metin1.mp3",
  "rect": { "x": 0.85, "y": 0.32, "width": 0.08, "height": 0.08 }
}
```

### 1.3 Video Demonstration (`video`)
Opens a video player with touch-friendly controls (play, pause, seek, speed).

```json
{
  "id": "video-1",
  "type": "video",
  "title": "Deney / Çözüm Videosu",
  "resource": "assets/video/deney.mp4",
  "rect": { "x": 0.85, "y": 0.44, "width": 0.08, "height": 0.08 }
}
```

### 1.4 Interactive Activity (`activity`)
Loads an interactive quiz or exercise modal from an activity definition file.

```json
{
  "id": "act-1",
  "type": "activity",
  "title": "Kavrama Testi",
  "activityFile": "activities/quizzes/q1.json",
  "rect": { "x": 0.85, "y": 0.56, "width": 0.08, "height": 0.08 }
}
```

---

## 2. Activity JSON Formats

### 2.1 Multiple Choice Activity (`multiple_choice`)

```json
{
  "type": "multiple_choice",
  "id": "quiz-1",
  "title": "İkinci Dereceden Denklemler Testi",
  "question": "x² - 5x + 6 = 0 denkleminin çözüm kümesi aşağıdakilerden hangisidir?",
  "options": [
    "{1, 6}",
    "{2, 3}",
    "{-2, -3}",
    "{0, 5}"
  ],
  "correctAnswerIndex": 1,
  "explanation": "x² - 5x + 6 = (x - 2)(x - 3) = 0 olduğundan kökler x = 2 ve x = 3'tür.",
  "shuffle": false
}
```

**Teacher Progressive Reveal**:
In classroom settings, teachers can click "Cevabı Göster / Çözüm" to instantly reveal the correct option and educational explanation to the class without forcing student input.

### 2.2 Matching Activity (`matching`)

```json
{
  "type": "matching",
  "id": "match-1",
  "title": "Matematiksel Terimler Eşleştirmesi",
  "prompt": "Sol taraftaki kavramları sağ taraftaki tanımlarıyla eşleştiriniz.",
  "pairs": [
    { "left": "Boş Küme", "right": "Elemanı olmayan küme (∅)" },
    { "left": "Evrensel Küme", "right": "Tüm elemanları içeren referans küme" },
    { "left": "Ayrık Kümeler", "right": "Kesişimi boş küme olan kümeler" }
  ]
}
```

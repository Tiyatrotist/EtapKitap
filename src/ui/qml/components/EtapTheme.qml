pragma Singleton
import QtQuick

QtObject {
    id: theme

    // Palette: Clean, modern, academic, high-contrast
    readonly property color primary: "#1E5F8A"          // MEB/ETAP blue
    readonly property color primaryDark: "#144260"
    readonly property color primaryLight: "#2980B9"
    readonly property color accent: "#E67E22"           // Warm amber
    readonly property color success: "#27AE60"          // Green for correct answers
    readonly property color error: "#C0392B"            // Red for errors / teacher alert

    // Surfaces
    readonly property color background: "#F5F7FA"       // Neutral soft grey
    readonly property color surface: "#FFFFFF"
    readonly property color surfaceDark: "#1E222B"
    readonly property color overlayMask: "#D0000000"     // Dimming mask
    readonly property color border: "#E1E6EB"
    readonly property color divider: "#ECF0F4"

    // Text colors
    readonly property color textPrimary: "#2C3E50"
    readonly property color textSecondary: "#7F8C8D"
    readonly property color textOnPrimary: "#FFFFFF"

    // Smartboard Touch Targets (Minimum 48px, primary tools 56-64px)
    readonly property int touchTargetSmall: 48
    readonly property int touchTargetMedium: 56
    readonly property int touchTargetLarge: 64

    // Radii
    readonly property int radiusSmall: 6
    readonly property int radiusMedium: 10
    readonly property int radiusLarge: 16

    // Fonts
    readonly property string fontFamily: "Segoe UI, Ubuntu, DejaVu Sans, sans-serif"
    readonly property int fontSizeSmall: 12
    readonly property int fontSizeBody: 14
    readonly property int fontSizeSubheading: 16
    readonly property int fontSizeHeading: 20
    readonly property int fontSizeLarge: 26
}

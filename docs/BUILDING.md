# Building EtapKitap from Source

This guide covers building EtapKitap on Linux (Pardus ETAP / Debian / Ubuntu) and Windows.

---

## 1. Prerequisites

### Linux (Pardus ETAP 23, Debian 12+, Ubuntu 22.04+)

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    ninja-build \
    pkg-config \
    qt6-base-dev \
    qt6-declarative-dev \
    qt6-multimedia-dev \
    qt6-tools-dev \
    libqt6svg6-dev \
    libqt6sql6-sqlite \
    libsqlite3-dev \
    zlib1g-dev \
    libqt6pdf6-dev
```

### Windows

- Visual Studio 2022 Community (Desktop development with C++, MSVC v143 toolset)
- Qt 6.4 or newer (e.g. Qt 6.11 MSVC 2022 64-bit)
- CMake 3.22+
- Ninja build tool

---

## 2. Compiling

### Linux Build

```bash
# Clone the repository
git clone https://github.com/Tiyatrotist/EtapKitap.git
cd EtapKitap

# Configure
cmake -B build -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DETAPKITAP_BUILD_TESTS=ON

# Compile
cmake --build build --parallel

# Run tests
ctest --test-dir build --output-on-failure
```

### Windows Build (via Visual Studio Developer Command Prompt)

```cmd
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DETAPKITAP_BUILD_TESTS=ON
cmake --build build --parallel

ctest --test-dir build --output-on-failure
```

---

## 3. Running EtapKitap

Once compiled, launch the executable directly or pass a PDF or `.akbook` file:

```bash
# Linux
./build/src/etapkitap demo/matematik9.akbook

# Windows
.\build\src\etapkitap.exe demo\matematik9.akbook
```

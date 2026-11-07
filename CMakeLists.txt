cmake_minimum_required(VERSION 3.22)
project(EtapKitap
    VERSION 1.0.0
    DESCRIPTION "Pardus ETAP Etkilesimli Dijital Ders Kitabi ve Akilli Tahta Calisma Ortami"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /permissive- /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(Qt6 6.4 REQUIRED COMPONENTS
    Core
    Gui
    Quick
    Qml
    QuickControls2
    Sql
    Svg
    Concurrent
)

find_package(Qt6 COMPONENTS Pdf PdfQuick QUIET)
find_package(Qt6 COMPONENTS Multimedia QUIET)
find_package(ZLIB REQUIRED)

qt_standard_project_setup()
if(COMMAND qt_policy)
    qt_policy(SET QTP0004 OLD)
endif()

option(ETAPKITAP_BUILD_TESTS "Build test suite" ON)

add_subdirectory(src)

if(ETAPKITAP_BUILD_TESTS)
    enable_testing()
    find_package(Qt6 REQUIRED COMPONENTS Test)
    add_subdirectory(tests)
endif()

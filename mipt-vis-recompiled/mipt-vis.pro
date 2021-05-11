
TEMPLATE = app
LANGUAGE = C++

TARGET = mipt-vis

VER_MAJ = 1
VER_MIN = 0
VER_PAT = 0

VERSION = -1.0.0

win32 {
DEFINES += MSWIN
}

DEFINES += MIPT_VIS_VER_MAJ=$$VER_MAJ MIPT_VIS_VER_MIN=$$VER_MIN MIPT_VIS_VER_PAT=$$VER_PAT

CONFIG += qt warn_on release

SRC_DIR = src

include ( src/application.pri )

INCLUDEPATH = src /usr/include/libxml2
OBJECTS_DIR = build
UI_DIR = build
MOC_DIR = build
RCC_DIR = build

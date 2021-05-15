
TEMPLATE = app
LANGUAGE = C++

TARGET = showgraph

VER_MAJ = 1
VER_MIN = 0
VER_PAT = 0

VERSION = -1.0.0

win32 {
DEFINES += MSWIN
}

# compilation flags

win32: {
  CFLAGS+=-wd4013 -wd4090
  QMAKE_CXXFLAGS += $${CFLAGS}
  QMAKE_CFLAGS += $${CFLAGS}
}
else: {
  CFLAGS+=-Wall \
          -fdiagnostics-show-option \
          -Wno-format \
          -Wno-parentheses \
          -Wno-sign-compare \
          -Wno-uninitialized \
          -Wno-unused-variable \
          -Wno-unused-function
  QMAKE_CXXFLAGS_WARN_ON += $${CFLAGS}
  QMAKE_CFLAGS_WARN_ON += $${CFLAGS}
}

DEFINES += SHOWGRAPH_VER_MAJ=$$VER_MAJ SHOWGRAPH_VER_MIN=$$VER_MIN SHOWGRAPH_VER_PAT=$$VER_PAT

CONFIG += qt warn_on release

SRC_DIR = src

include ( src/application.pri )

RESOURCES += src/Gui/images.qrc

OBJECTS_DIR = build
UI_DIR = build
MOC_DIR = build
RCC_DIR = build
QT += gui widgets xml printsupport



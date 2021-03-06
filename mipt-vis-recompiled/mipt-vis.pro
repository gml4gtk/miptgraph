
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

DEFINES += MIPT_VIS_VER_MAJ=$$VER_MAJ MIPT_VIS_VER_MIN=$$VER_MIN MIPT_VIS_VER_PAT=$$VER_PAT

CONFIG += qt warn_on release

SRC_DIR = src

include ( src/application.pri )

INCLUDEPATH = src /usr/include/libxml2
OBJECTS_DIR = build
UI_DIR = build
MOC_DIR = build
RCC_DIR = build
QT += widgets
LIBS += -L/usr/lib/x86_64-linux-gnu -lxml2

# -------------------------------------------------
# Project created by QtCreator 2010-09-28T22:50:23
# -------------------------------------------------
system(ccache -V):QMAKE_CXX = ccache g++

TARGET = leterrier-fubuki

QT += script xml network

TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    abuleduexercicev0.cpp \
    abuleduaproposv0.cpp

HEADERS += mainwindow.h \
    abuleduexercicev0.h \
    abuleduaproposv0.h

FORMS += mainwindow.ui \
    abuleduaproposv0.ui

RESOURCES += Fubuki.qrc

#AbulEduLangV1
include(lib/abuledulangv1/abuledulangv1.pri)
include(lang/lang.pri)

RC_FILE = windows/windows.rc

win32 {
    DEFINES      += __WINDOWS__=1
    OTHER_FILES  += windows/windows.rc
    RC_FILE       = windows/windows.rc
}

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

TRANSLATIONS = lang/fubuki_oc.ts \
    lang/fubuki_en.ts \
    lang/fubuki_de.ts \
    lang/fubuki_es.ts \
    lang/fubuki_nl.ts \
    lang/fubuki_ca.ts \
    lang/fubuki_br.ts

RC_FILE = windows/windows.rc

win32 {
    DEFINES      += __WINDOWS__=1
    OTHER_FILES  += windows/windows.rc
    RC_FILE       = windows/windows.rc
}

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

HEADERS += version.h \
    mainwindow.h \
    abuleduexercicev0.h \
    abuleduaproposv0.h

FORMS += mainwindow.ui \
    abuleduaproposv0.ui

RESOURCES += Fubuki.qrc


RC_FILE = windows/windows.rc

win32 {
    DEFINES      += __WINDOWS__=1
    OTHER_FILES  += windows/windows.rc
    RC_FILE       = windows/windows.rc
}

#AbulEduLangV1
include(lib/abuledulangv1/abuledulangv1.pri)
include(lang/lang.pri)
include(lib/abuleduapplicationv1/abuleduapplicationv1.pri)
include(lib/abuleduconstantesv1/abuleduconstantesv1.pri)
include(lib/abuledunetworkaccessmanagerv1/abuledunetworkaccessmanagerv1.pri)
include(lib/abuledumessageboxv1/abuledumessageboxv1.pri)
include(lib/abuleduidentitesv1/abuleduidentitesv1.pri)
include(lib/abuleduflatboutonv1/abuleduflatboutonv1.pri)
include(lib/maia/maia.pri)
include(lib/abuledufilev1/abuledufilev1.pri)
include(lib/abuledustackedwidgetv1/abuledustackedwidgetv1.pri)

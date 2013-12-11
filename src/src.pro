!android {
  system(ccache -V):QMAKE_CXX = ccache g++
}
android {
  DEFINES += QT_NO_PRINTER=1
  system(ccache -V):QMAKE_CXX = ccache arm-linux-gnueabi-g++-4.6
}

TARGET = leterrier-fubuki

QT += script xml network

TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    abuleduexercicev0.cpp \
    abuleduaproposv0.cpp \
    abuleduinputbox.cpp

HEADERS += version.h \
    mainwindow.h \
    abuleduexercicev0.h \
    abuleduaproposv0.h \
    abuleduinputbox.h

FORMS += mainwindow.ui \
    abuleduaproposv0.ui

RESOURCES += Fubuki.qrc

win32 {
    OTHER_FILES  += ../windows/windows.rc
    RC_FILE       = ../windows/windows.rc
}

macx {
 ICON = ../macos/icones/leterrier-fubuki.icns
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
include(lib/abuledumenufeuillev1/abuledumenufeuillev1.pri)
include(lib/abuledusingletonv1/abuledusingletonv1.pri)
include(lib/abuledupluginloaderv1/abuledupluginloaderv1.pri)
include(lib/abuleduaproposv1/abuleduaproposv1.pri)
include(lib/abuleduandroidsupportv1/abuleduandroidsupportv1.pri)

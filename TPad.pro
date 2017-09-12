#-------------------------------------------------
#
# Project created by QtCreator 2017-08-26T21:20:31
#
#-------------------------------------------------

QT       += core gui widgets printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TPad
TEMPLATE = app
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS = languages/TranslationTPad_en.ts  languages/TranslationTPad_de.ts languages/TranslationTPad_pt.ts

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
    src/about.cpp \
    src/translation.cpp \
    src/credits.cpp \
    src/save_load_files.cpp \
    src/highlighter.cpp

HEADERS += \
        src/mainwindow.h \
    src/about.h \
    src/credits.h \
    src/highlighter.h

FORMS += \
        src/mainwindow.ui \
    src/about.ui \
    src/credits.ui

RESOURCES += \
    src/icons.qrc


unix:!macx: LIBS += -lX11

unix {
  #TRANSLATIONS
  exists($$[QT_INSTALL_BINS]/lrelease) {
    TRANSLATIONS = $$system("find src/languages/ -name 'TranslationTPad_*.ts'")
    updateqm.input = TRANSLATIONS
    updateqm.output = src/languages/${QMAKE_FILE_BASE}.qm
    updateqm.commands = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm src/languages/${QMAKE_FILE_BASE}.qm
    updateqm.CONFIG += no_link target_predeps
    QMAKE_EXTRA_COMPILERS += updateqm
  }

#VARIABLES
  isEmpty(PREFIX) {
    PREFIX = /usr
  }
  BINDIR = $$PREFIX/bin
  DATADIR =$$PREFIX/share

  DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

  #MAKE INSTALL

  INSTALLS += target desktop trans

  target.path =$$BINDIR

  desktop.path = $$DATADIR/applications
  desktop.files += ./src/share/applications/$${TARGET}.desktop

  trans.path = $$DATADIR/TPad
  trans.files += src/languages
}

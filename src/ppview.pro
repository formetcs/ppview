#-------------------------------------------------
#
# Project created by QtCreator 2018-01-29T14:50:24
#
#-------------------------------------------------

QT       += core gui xml widgets printsupport

win32 {
    message(Building for Windows)
    DEFINES += _WINDOWS
}
unix {
    macx {
        message(Building for MacOS X)
        DEFINES += _MACOSX
    }
    else {
        message(Building for Linux)
        DEFINES += _LINUX
        CONFIG += linux
    }
}

TARGET = ppview
macx:TARGET = "PlanPro Viewer"
TEMPLATE = app

CONFIG += lrelease

macx:QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    anhang.cpp \
    basisobjekt.cpp \
    documenttreemodel.cpp \
    graphicsscenebuilder.cpp \
    main.cpp \
    mainwindow.cpp \
    objectinfowidget.cpp \
    objectlistmodel.cpp \
    filterwidget.cpp \
    planprodocument.cpp \
    planprograph.cpp \
    graphicsscene.cpp \
    domitem.cpp \
    planproxmldocument.cpp \
    preferences.cpp \
    preferencesdialog.cpp \
    punktobjekt.cpp \
    selectionmanager.cpp \
    smtverifierdialog.cpp \
    temporarypunktobjekt.cpp \
    textfiledialog.cpp \
    urobjekt.cpp

HEADERS += \
    anhang.h \
    basisobjekt.h \
    documenttreemodel.h \
    global.h \
    graphicsscenebuilder.h \
    mainwindow.h \
    objectinfowidget.h \
    objectlistmodel.h \
    filterwidget.h \
    planprodocument.h \
    planprograph.h \
    graphicsscene.h \
    domitem.h \
    planproxmldocument.h \
    preferences.h \
    preferencesdialog.h \
    punktobjekt.h \
    selectionmanager.h \
    smtverifierdialog.h \
    temporarypunktobjekt.h \
    textfiledialog.h \
    urobjekt.h \
    version.h

RESOURCES += \
    ppview.qrc

win32:RC_FILE = ppview.rc
macx:ICON     = ppview.icns

TRANSLATIONS  = ppview_en.ts \
                ppview_de.ts

FORMS += \
    finddialog.ui \
    preferencesdialog.ui \
    smtverifierdialog.ui
    

# Deployment Target for macOS
macx:macdeploytarget.target = macdeploy
macx:macdeploytarget.commands = $$[QT_INSTALL_BINS]/macdeployqt $$shell_quote($${TARGET}.app); \
                                test -d $$shell_quote($${TARGET}.app/Contents/Resources/translations) || mkdir -p $$shell_quote($${TARGET}.app/Contents/Resources/translations); \
                                cp -f $$[QT_INSTALL_TRANSLATIONS]/*.qm $$shell_quote($${TARGET}.app/Contents/Resources/translations); \
                                cp -f .qm/ppview_*.qm $$shell_quote($${TARGET}.app/Contents/Resources/translations); \
                                cp -f ../LICENSE ../third-party-licenses.txt ../README.md $$shell_quote($${TARGET}.app/Contents/Resources)
macx:macdeploytarget.depends = all
macx:QMAKE_EXTRA_TARGETS += macdeploytarget


# Install Set for Linux
isEmpty(PPVIEW_INSTALL_PATH) {
    PPVIEW_INSTALL_PATH = /usr/local/ppview
}
linux:QM_FILES_INSTALL_PATH = $${PPVIEW_INSTALL_PATH}/translations
linux:target.path = $${PPVIEW_INSTALL_PATH}
linux:readmefiles.path = $${PPVIEW_INSTALL_PATH}
linux:readmefiles.files = ../LICENSE ../third-party-licenses.txt ../README.md
linux:INSTALLS += target readmefiles

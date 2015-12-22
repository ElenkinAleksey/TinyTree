TEMPLATE = app

QT += qml quick multimedia widgets network xml
CONFIG += c++11 link_pkgconfig qt

SOURCES += main.cpp \
    player.cpp \
    presenter.cpp \
    filecontroller.cpp \
    webcontroller.cpp

PKGCONFIG += taglib


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    player.h \
    presenter.h \
    filecontroller.h \
    webcontroller.h


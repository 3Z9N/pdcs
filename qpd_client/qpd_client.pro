#
# qpd_client project
# Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)
#

QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qpd_client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

QMAKE_CXXFLAGS += -O2

unix:INCLUDEPATH += "../pd_client"
win32:INCLUDEPATH += "../pd_client"

unix:LIBS += -L../pd_client/build/debug -lpd_client -lboost_filesystem -lboost_program_options
win32:LIBS += -L../pd_client/build/debug -lpd_client -lboost_filesystem-mt -lboost_program_options-mt

# Build config
CONFIG += debug

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
#MOC_DIR = $$DESTDIR/.moc
#RCC_DIR = $$DESTDIR/.qrc
#UI_DIR = $$DESTDIR/.ui

# Input files
SOURCES += \
        main.cpp \
        mainwin.cpp \
    connectdialog.cpp \
    eventdialog.cpp

HEADERS += \
        mainwin.h \
    connectdialog.h \
    eventtablemodel.h \
    eventdialog.h

FORMS += \
        mainwin.ui \
    connectdialog.ui \
    eventdialog.ui


# Install
target.path = /usr/local/bin
INSTALLS += target


# Default rules for deployment.
#qnx: target.path = /tmp/$${TARGET}/bin
#else: unix:!android: target.path = /opt/$${TARGET}
#!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    build_script \
    README.md \
    Doxyfile

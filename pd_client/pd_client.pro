#
# pd_client project
# Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)
#

TEMPLATE = lib
TARGET = pd_client
INCLUDEPATH += .
QT -= core gui

# Linux libs
unix:LIBS += -lpthread -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread -lcryptopp

# Windows libs (MSYS2 mingw)
win32::LIBS += -lws2_32 -lwsock32 -lboost_system-mt -lboost_filesystem-mt -lboost_program_options-mt -lcryptopp

# Input
HEADERS += pd_client.h pd_message.h pd_tools.h
SOURCES += pd_client.cpp pd_message.cpp pd_tools.cpp

# Build config
CONFIG += debug

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui


# Install
target.path = /usr/local/lib
INSTALLS += target

incfiles.path = /usr/local/include/pd_client
incfiles.files = pd_client.h pd_message.h pd_tools.h
INSTALLS += incfiles


# Distribution
DISTFILES += \
    Doxyfile \
    README.md \
    build_script

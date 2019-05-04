#
# pd_server project
# Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)
#

TEMPLATE = app
TARGET = pd_server
INCLUDEPATH += .
QT -= gui core

# Linux libs
unix:LIBS += -lpthread -lboost_system -lboost_filesystem -lboost_program_options -lcryptopp -lsqlite3

# Windows libs (MSYS2 mingw)
win32::LIBS += -lws2_32 -lwsock32 -lboost_system-mt -lboost_filesystem-mt -lboost_program_options-mt -lcryptopp -lsqlite3

# Input
HEADERS += pd_message.h pd_server.h pd_sqlite.h pd_tools.h
SOURCES += pd_message.cpp pd_server.cpp pd_sqlite.cpp pd_tools.cpp

# Build config
CONFIG += debug

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj

# Install
target.path = /opt/pd_server
INSTALLS += target

#postinst.path = /opt
#postinst.commands = chmod -R go-rwx /opt/pd_server
#INSTALLS += postinst

DISTFILES += \
    Doxyfile \
    README.md \
    build_script

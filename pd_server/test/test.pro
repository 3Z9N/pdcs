#
# pd_server project
# Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)
#

TEMPLATE = app
TARGET = test
INCLUDEPATH += . ../
QT -= gui core

# Linux libs
unix:LIBS += -lpthread -lboost_system -lboost_filesystem -lboost_program_options -lboost_unit_test_framework -lcryptopp -lsqlite3

# Windows libs (MSYS2 mingw)
win32::LIBS += -lws2_32 -lwsock32 -lboost_system-mt -lboost_filesystem-mt -lboost_program_options-mt -lboost_unit_test_framework-mt -lcryptopp -lsqlite3

#DEFINES += 

# Input
HEADERS += \
    ../pd_message.h \
    ../pd_sqlite.h \
    ../pd_tools.h
SOURCES += test.cpp \
    ../pd_sqlite.cpp \
    ../pd_message.cpp \
    ../pd_tools.cpp

DISTFILES += \
    build_script \
    README.md



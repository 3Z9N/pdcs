#
# pd_server project
# Author: Krzysztof Przybylski (krzysztof.m.przybylski@gmail.com)
#

TEMPLATE = app
TARGET = test
INCLUDEPATH += . ../
QT -= gui core

# Linux libs
unix:LIBS += -lpthread -lboost_system -lboost_filesystem -lboost_program_options -lboost_thread -lboost_unit_test_framework -lcryptopp

# Windows libs (MSYS2 mingw)
win32::LIBS += -lws2_32 -lwsock32 -lboost_system-mt -lboost_filesystem-mt -lboost_program_options-mt -lboost_thread -lboost_unit_test_framework-mt -lcryptopp

#DEFINES += 

# Input
HEADERS += \
    ../pd_message.h \
    ../pd_tools.h \
    ../pd_client.h
SOURCES += test.cpp \
    ../pd_message.cpp \
    ../pd_tools.cpp \
    ../pd_client.cpp

# Distribution
DISTFILES += \
    README.md \
    build_script



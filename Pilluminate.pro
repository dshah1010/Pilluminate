QT += core gui widgets

CONFIG += c++11

TARGET = Pilluminate
TEMPLATE = app

SOURCES += src/interfaces/UserInterface.cpp \
           src/models/VirtualLED.cpp \
           src/main.cpp

HEADERS += include/interfaces/UserInterface.h \
           include/models/VirtualLED.h \

# Add the include path for headers
INCLUDEPATH += $$PWD/include


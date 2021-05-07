TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/Includes

SOURCES += \
        Sources/Tools/Blowfish.cpp \
        Sources/Tools/Helpers.cpp \
        main.cpp

HEADERS += \
    Includes/MH4U/Keys.hpp \
    Includes/Tools/Blowfish.hpp \
    Includes/Tools/BlowfishTables.hpp \
    Includes/Tools/Helpers.hpp \
    Includes/types.h

TEMPLATE = app

LIBS += -lz -lstdc++fs
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/Includes

SOURCES += \
        Sources/MH4U/MH4U.cpp \
        Sources/Tools/ARC.cpp \
        Sources/Tools/Blowfish.cpp \
        Sources/Tools/CContainer.cpp \
        Sources/Tools/Utils.cpp \
        main.cpp

HEADERS += \
    Includes/MH4U/MH4U.hpp \
    Includes/Tools/ARC.hpp \
    Includes/Tools/Blowfish.hpp \
    Includes/Tools/BlowfishTables.hpp \
    Includes/Tools/CContainer.hpp \
    Includes/Tools/Utils.hpp \
    Includes/types.h

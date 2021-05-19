TEMPLATE = app

LIBS += -lz -lstdc++fs
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/Includes

SOURCES += \
        Sources/Global.cpp \
        Sources/MH4U/MH4U.cpp \
        Sources/MHXX/Extentions/gmd.cpp \
        Sources/MHXX/Extentions/qdp.cpp \
        Sources/MHXX/Extentions/sem.cpp \
        Sources/Tools/ARC.cpp \
        Sources/Tools/Blowfish.cpp \
        Sources/Tools/CContainer.cpp \
        Sources/Tools/Utils.cpp \
        main.cpp

HEADERS += \
    Includes/Global.hpp \
    Includes/MH4U/MH4U.hpp \
    Includes/MH4U/Resource.hpp \
    Includes/MHXX/Extentions/gmd.hpp \
    Includes/MHXX/Extentions/qdp.hpp \
    Includes/MHXX/Extentions/sem.hpp \
    Includes/MHXX/MHXX.hpp \
    Includes/Tools/ARC.hpp \
    Includes/Tools/Blowfish.hpp \
    Includes/Tools/BlowfishTables.hpp \
    Includes/Tools/CContainer.hpp \
    Includes/Tools/Test.hpp \
    Includes/Tools/Utils.hpp \
    Includes/types.h

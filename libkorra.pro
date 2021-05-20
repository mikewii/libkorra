TEMPLATE = app

LIBS += -lz -lstdc++fs
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += $$PWD/Includes

SOURCES += \
        Sources/Global.cpp \
        Sources/MH4U/MH4U.cpp \
        Sources/MHXX/Enemy/Enemy.cpp \
        Sources/MHXX/Extentions/esl.cpp \
        Sources/MHXX/Extentions/ext.cpp \
        Sources/MHXX/Extentions/gmd.cpp \
        Sources/MHXX/Extentions/qdl.cpp \
        Sources/MHXX/Extentions/qdp.cpp \
        Sources/MHXX/Extentions/rem.cpp \
        Sources/MHXX/Extentions/sem.cpp \
        Sources/MHXX/Extentions/sup.cpp \
        Sources/Tools/ARC.cpp \
        Sources/Tools/Blowfish.cpp \
        Sources/Tools/CContainer.cpp \
        Sources/Tools/Pair.cpp \
        Sources/Tools/Utils.cpp \
        main.cpp

HEADERS += \
    Includes/Global.hpp \
    Includes/MH4U/MH4U.hpp \
    Includes/MH4U/Resource.hpp \
    Includes/MHXX/Enemy/Enemy.hpp \
    Includes/MHXX/Extentions/esl.hpp \
    Includes/MHXX/Extentions/ext.hpp \
    Includes/MHXX/Extentions/gmd.hpp \
    Includes/MHXX/Extentions/qdl.hpp \
    Includes/MHXX/Extentions/qdp.hpp \
    Includes/MHXX/Extentions/rem.hpp \
    Includes/MHXX/Extentions/sem.hpp \
    Includes/MHXX/Extentions/sup.hpp \
    Includes/MHXX/MHXX.hpp \
    Includes/MHXX/Strings/Enemy.hpp \
    Includes/MHXX/Strings/Items.hpp \
    Includes/Tools/ARC.hpp \
    Includes/Tools/Blowfish.hpp \
    Includes/Tools/BlowfishTables.hpp \
    Includes/Tools/CContainer.hpp \
    Includes/Tools/Pair.hpp \
    Includes/Tools/Test.hpp \
    Includes/Tools/Utils.hpp \
    Includes/types.h

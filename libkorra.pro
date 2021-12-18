QT      += core gui

# console
# TEMPLATE = app
# CONFIG  += console c++11
# CONFIG  -= app_bundle
# CONFIG  -= qt

LIBS    += -lz -lprocps
CONFIG  += c++11


INCLUDEPATH += $$PWD/Includes

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
        GUI/gmd_gui.cpp \
        GUI/mainwindow.cpp \
        GUI/worker.cpp \
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
        Sources/Tools/Task.cpp \
        Sources/Tools/Test.cpp \
        Sources/Tools/Utils.cpp \
        main.cpp

HEADERS += \
    GUI/gmd_gui.h \
    GUI/mainwindow.h \
    GUI/worker.h \
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
    Includes/MHXX/Quest/Common.hpp \
    Includes/MHXX/Strings/Common.hpp \
    Includes/MHXX/Strings/Enemy.hpp \
    Includes/MHXX/Strings/Items.hpp \
    Includes/MHXX/Strings/ext.hpp \
    Includes/Tools/ARC.hpp \
    Includes/Tools/Blowfish.hpp \
    Includes/Tools/BlowfishTables.hpp \
    Includes/Tools/CContainer.hpp \
    Includes/Tools/Pair.hpp \
    Includes/Tools/Task.hpp \
    Includes/Tools/Test.hpp \
    Includes/Tools/Utils.hpp \
    Includes/types.h

FORMS += \
    GUI/gmd_gui.ui \
    GUI/mainwindow.ui

# Default rules for deployment.
# copypaste from default QT Widgets app
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

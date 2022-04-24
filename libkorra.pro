QT      += core gui

# console
# TEMPLATE = app
# CONFIG  += console c++11
# CONFIG  -= app_bundle
# CONFIG  -= qt

LIBS    += -lz
CONFIG  += c++17


INCLUDEPATH += $$PWD/include

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
    GUI/gui_gmd.cpp \
    GUI/gui_lmd.cpp \
    GUI/mainwindow.cpp \
    include/MHXX/Savefile.cpp \
    main.cpp \
    src/Global.cpp \
    src/MH4U/Extentions/lmd.cpp \
    src/MH4U/MH4U.cpp \
    src/MH4U/MH4U_Test.cpp \
    src/MH4U/Quest.cpp \
    src/MHXX/Enemy/Enemy.cpp \
    src/MHXX/Extentions/esl.cpp \
    src/MHXX/Extentions/ext.cpp \
    src/MHXX/Extentions/gmd.cpp \
    src/MHXX/Extentions/qdl.cpp \
    src/MHXX/Extentions/qdp.cpp \
    src/MHXX/Extentions/rem.cpp \
    src/MHXX/Extentions/sem.cpp \
    src/MHXX/Extentions/sup.cpp \
    src/MHXX/MHXX_Test.cpp \
    src/MHXX/Quest/Common.cpp \
    src/Tools/ARC.cpp \
    src/Tools/Blowfish.cpp \
    src/Tools/CContainer.cpp \
    src/Tools/File.cpp \
    src/Tools/Folder.cpp \
    src/Tools/Pair.cpp \
    src/Tools/Task.cpp \
    src/Tools/Test.cpp \
    src/Tools/Utils.cpp

HEADERS += \
    GUI/gui_gmd.hpp \
    GUI/gui_lmd.h \
    GUI/mainwindow.h \
    include/Global.hpp \
    include/MH4U/Equipment/Hunter/Armor/Arms.hpp \
    include/MH4U/Equipment/Hunter/Armor/Chest.hpp \
    include/MH4U/Equipment/Hunter/Armor/Head.hpp \
    include/MH4U/Equipment/Hunter/Armor/Legs.hpp \
    include/MH4U/Equipment/Hunter/Armor/Waist.hpp \
    include/MH4U/Equipment/Hunter/Weapon/Bow.hpp \
    include/MH4U/Equipment/Hunter/Weapon/CB.hpp \
    include/MH4U/Equipment/Hunter/Weapon/DB.hpp \
    include/MH4U/Equipment/Hunter/Weapon/GL.hpp \
    include/MH4U/Equipment/Hunter/Weapon/GS.hpp \
    include/MH4U/Equipment/Hunter/Weapon/HBG.hpp \
    include/MH4U/Equipment/Hunter/Weapon/HH.hpp \
    include/MH4U/Equipment/Hunter/Weapon/Hammer.hpp \
    include/MH4U/Equipment/Hunter/Weapon/IG.hpp \
    include/MH4U/Equipment/Hunter/Weapon/IG_Kinsect.hpp \
    include/MH4U/Equipment/Hunter/Weapon/LBG.hpp \
    include/MH4U/Equipment/Hunter/Weapon/LS.hpp \
    include/MH4U/Equipment/Hunter/Weapon/Lance.hpp \
    include/MH4U/Equipment/Hunter/Weapon/SA.hpp \
    include/MH4U/Equipment/Hunter/Weapon/SNS.hpp \
    include/MH4U/Equipment/Palico/Armor/Palico_chest.hpp \
    include/MH4U/Equipment/Palico/Armor/Palico_helmet.hpp \
    include/MH4U/Equipment/Palico/Weapon/Palico_weapons.hpp \
    include/MH4U/Extentions/lmd.hpp \
    include/MH4U/Items.hpp \
    include/MH4U/MH4U.hpp \
    include/MH4U/MH4U_Test.hpp \
    include/MH4U/Quest.hpp \
    include/MH4U/Resource.hpp \
    include/MHXX/Enemy/Enemy.hpp \
    include/MHXX/Extentions/esl.hpp \
    include/MHXX/Extentions/ext.hpp \
    include/MHXX/Extentions/gmd.hpp \
    include/MHXX/Extentions/qdl.hpp \
    include/MHXX/Extentions/qdp.hpp \
    include/MHXX/Extentions/rem.hpp \
    include/MHXX/Extentions/sem.hpp \
    include/MHXX/Extentions/sup.hpp \
    include/MHXX/MHXX.hpp \
    include/MHXX/MHXX_Test.hpp \
    include/MHXX/Quest/Common.hpp \
    include/MHXX/Savefile.hpp \
    include/MHXX/Strings/Common.hpp \
    include/MHXX/Strings/Items.hpp \
    include/MHXX/Strings/ext.hpp \
    include/Tools/ARC.hpp \
    include/Tools/Blowfish.hpp \
    include/Tools/BlowfishTables.hpp \
    include/Tools/CContainer.hpp \
    include/Tools/File.hpp \
    include/Tools/Folder.hpp \
    include/Tools/Pair.hpp \
    include/Tools/TEX.hpp \
    include/Tools/Task.hpp \
    include/Tools/Test.hpp \
    include/Tools/Utils.hpp \
    include/types.h

# Default rules for deployment.
# copypaste from default QT Widgets app
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    GUI/gui_gmd.ui \
    GUI/gui_lmd.ui \
    GUI/mainwindow.ui

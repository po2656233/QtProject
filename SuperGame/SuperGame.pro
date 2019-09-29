#-------------------------------------------------
#
# Project created by QtCreator 2019-04-22T21:10:11
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11 resources_big
TARGET = SuperGame
TEMPLATE = app
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#DEFINES += PROTOBUF_USE_DLLS
SOURCES += \
        main.cpp \
   base/network/protobuffer/include/baccarat.pb.cc \
   base/network/protobuffer/include/comand.pb.cc \
   base/network/protobuffer/include/cowcow.pb.cc \
   base/network/protobuffer/include/fishLord.pb.cc \
   base/network/protobuffer/include/gamecomm.pb.cc \
   base/network/protobuffer/include/landLords.pb.cc \
   base/network/protobuffer/include/login.pb.cc \
   base/network/protobuffer/include/mahjong.pb.cc \
   base/network/handlemsg.cpp \
    login/loginui.cpp \
    login/platform/gameframeui.cpp \
    superman.cpp \
    login/platform/rooms/games/baccarat/baccaratui.cpp \
    base/baseui/uibase.cpp \
    base/baseui/gamemap.cpp \
    login/platform/rooms/games/landlord/landlordui.cpp \
    base/baseui/uilabel.cpp \
    login/platform/rooms/games/mahjong/mahjongui.cpp \
    login/platform/rooms/games/cowcow/cowcowui.cpp \
    login/platform/rooms/games/landlord/landlordlogic.cpp \
    login/platform/rooms/games/baccarat/baccaratlogic.cpp \
    base/baseui/uibetarea.cpp \
    base/baseui/uitextscroll.cpp \
    login/platform/rooms/games/chineseChess/chinesechessui.cpp \
    login/platform/rooms/games/chineseChess/chinesechesslogic.cpp \
    base/baseui/uimeterprocessbar.cpp \
    login/platform/rooms/games/landlord/playerui.cpp \
    login/platform/rooms/games/toubao/toubaoui.cpp \
    login/platform/rooms/games/cowcow/cowcowlogic.cpp \
    login/platform/rooms/games/zhajinhua/zhajinhuaui.cpp \
    base/gameunit.cpp \
    login/platform/rooms/homeui.cpp \
    login/platform/rooms/loadingui.cpp \
    base/baseui/uihandcard.cpp \
    base/baseui/uicard.cpp \
    base/baseui/gameproperty.cpp

HEADERS += \
    base/network/protobuffer/include/baccarat.pb.h \
    base/network/protobuffer/include/comand.pb.h \
    base/network/protobuffer/include/cowcow.pb.h \
    base/network/protobuffer/include/fishLord.pb.h \
    base/network/protobuffer/include/gamecomm.pb.h \
    base/network/protobuffer/include/landLords.pb.h \
    base/network/protobuffer/include/login.pb.h \
    base/network/protobuffer/include/mahjong.pb.h \
    base/global.h \
    base/Magic_Singleton.h \
    base/network/handlemsg.h \
    login/loginui.h \
    login/platform/gameframeui.h \
    login/platform/rooms/homeui.h \
    login/platform\rooms/loadingui.h \
    superman.h \
    login/platform/rooms/games/baccarat/baccaratui.h \
    base/baseui/uibase.h \
    base/baseui/gamemap.h \
    login/platform/rooms/games/landlord/landlordui.h \
    base/common.h \
    base/baseui/uilabel.h \
    login/platform/rooms/games/mahjong/mahjongui.h \
    login/platform/rooms/games/cowcow/cowcowui.h \
    login/platform/rooms/games/landlord/landlordlogic.h \
    login/platform/rooms/games/baccarat/baccaratlogic.h \
    base/baseui/uibetarea.h \
    base/baseui/uitextscroll.h \
    login/platform/rooms/games/chineseChess/chinesechessui.h \
    login/platform/rooms/games/chineseChess/chinesechesslogic.h \
    base/baseui/uimeterprocessbar.h \
    login/platform/rooms/games/landlord/playerui.h \
    login/platform/rooms/games/toubao/toubaoui.h \
    login/platform/rooms/games/cowcow/cowcowlogic.h \
    login/platform/rooms/games/zhajinhua/zhajinhuaui.h \
    base/gameunit.h \
    base/baseui/uihandcard.h \
    base/baseui/uicard.h \
    base/baseui/gameproperty.h


FORMS += \
        src/ui/mainwindow.ui \
    src/ui/loginui.ui \
    src/ui/gameframeui.ui \
    src/ui/homeui.ui \
    src/ui/loadingui.ui \
    login/platform/rooms/games/baccarat/baccaratui.ui \
    login/platform/rooms/games/landlord/landlordui.ui \
    login/platform/rooms/games/mahjong/mahjong.ui \
    login/platform/rooms/games/cowcow/cowcowui.ui \
    login/platform/rooms/games/chineseChess/chinesechessui.ui \
    login/platform/rooms/games/landlord/playerui.ui \
    login/platform/rooms/games/toubao/toubaoui.ui \
    login/platform/rooms/games/zhajinhua/zhajinhuaui.ui \
    src/ui/register.ui


win32: LIBS += -L$$PWD/base/network/protobuffer/lib/ -lprotobuf

INCLUDEPATH += $$PWD/base/network/protobuffer/include
DEPENDPATH += $$PWD/base/network/protobuffer/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/base/network/protobuffer/lib/protobuf.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/base/network/protobuffer/lib/libprotobuf.a

DISTFILES +=

RESOURCES += \
    src/element.qrc

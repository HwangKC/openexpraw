#-------------------------------------------------
#
# Project created by QtCreator 2020-12-02T14:51:29
#
#-------------------------------------------------
QT       += core
QT       -=  gui

TARGET = expraw
TEMPLATE = lib

macos:QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.13

DEFINES += EXPRAW_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# libraw include path
win32:INCLUDEPATH += $$PWD/../libraw/
macos:INCLUDEPATH += $$PWD/../libraw/

# opencv include/lib path
win32:INCLUDEPATH += $$PWD/../../sdk/win/opencv_sdk/include/
win32:LIBS += -L$$PWD/../../sdk/win/opencv_sdk/x64/vc14/lib/
macos:INCLUDEPATH += $$PWD/../../sdk/opencv_sdk/include/opencv4/
macos:LIBS += -L$$PWD/../../sdk/opencv_sdk/lib/

CONFIG(debug, debug|release):{
win32:LIBS += -lopencv_core341d
macos:LIBS += -lm -lopencv_core
}
else{
win32:LIBS += -lopencv_core341
macos:LIBS += -lm -lopencv_core
}

# expraw library conigure
CONFIG(debug, debug|release):{
message("Linking DEBUG version dynamic library")
win32:LIBS += -L$$PWD/../buildfiles/debug-x86_64/ -llibraw
win32:DESTDIR = $$PWD/../buildfiles/debug-x86_64/
macos:LIBS += -L$$PWD/../../sdk/lib/ -lraw
macos:DESTDIR = $$PWD/../lib/
TARGET = exprawd
}
else{
message("Linking RELEASE version dynamic library")
win32:LIBS += -L$$PWD/../buildfiles/release-x86_64/ -llibraw
win32:DESTDIR = $$PWD/../buildfiles/release-x86_64/
macos:LIBS += -L$$PWD/../../sdk/lib/ -lraw
macos:DESTDIR = $$PWD/../lib/
TARGET = expraw
}

SOURCES += \
        expraw.cpp

HEADERS += \
        expraw.h \
        expraw_global.h 

unix {
    target.path = /usr/lib
    INSTALLS += target
}

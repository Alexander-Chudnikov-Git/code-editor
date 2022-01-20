QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 0.0.0.2
QMAKE_TARGET_COMPANY = Alexander Chudnikov
QMAKE_TARGET_PRODUCT = Code Editor
QMAKE_TARGET_DESCRIPTION = Code Editor
QMAKE_TARGET_COPYRIGHT = Alexander Chudnikov

ICON = :/ico/image/main.ico

win32
{
    RC_FILE += recources.rc
    OTHER_FILES += recources.rc
    QTPLUGIN.audio=qtaudio_windows
}

CONFIG += c++14
CONFIG += static

QMAKE_LFLAGS_RELEASE += -static -static-libgcc -Os

TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutwindow.cpp \
    colorswitchbutton.cpp \
    editorwidget.cpp \
    filerepresentation.cpp \
    highlighter.cpp \
    main.cpp \
    mainwindow.cpp \
    syntaxselectwidget.cpp

HEADERS += \
    aboutwindow.h \
    colorswitchbutton.h \
    editorwidget.h \
    filerepresentation.h \
    highlighter.h \
    mainwindow.h \
    syntaxselectwidget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

buildtimeTarget.target = aboutwindow.h
buildtimeTarget.depends = FORCE
build_nr.commands = del /f C:/Users/CHOODICK/Documents/Code_Editor/aboutwindow.h
PRE_TARGETDEPS += aboutwindow.h
QMAKE_EXTRA_TARGETS += buildtimeTarget

RESOURCES += \
    recources.qrc

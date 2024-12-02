QT       += core gui
QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    authentificationmanager.cpp \
    cachemanager.cpp \
    cloudgallerywindow.cpp \
    directory.cpp \
    file.cpp \
    filesystem.cpp \
    filetransfermanager.cpp \
    foldertransfermanager.cpp \
    main.cpp \
    mainwindow.cpp \
    memorymanager.cpp \
    page.cpp \
    pagetable.cpp \
    segment.cpp \
    segmentmanager.cpp \
    signinwindow.cpp \
    signupwindow.cpp \
    welcomepage.cpp

HEADERS += \
    authentificationmanager.h \
    cachemanager.h \
    cloudgallerywindow.h \
    directory.h \
    file.h \
    filesystem.h \
    filetransfermanager.h \
    foldertransfermanager.h \
    mainwindow.h \
    memorymanager.h \
    page.h \
    pagetable.h \
    segment.h \
    segmentmanager.h \
    signinwindow.h \
    signupwindow.h \
    welcomepage.h

FORMS += \
    cloudgallerywindow.ui \
    mainwindow.ui \
    signinwindow.ui \
    signupwindow.ui \
    welcomepage.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

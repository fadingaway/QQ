#-------------------------------------------------
#
# Project created by QtCreator 2018-10-11T15:41:06
#
#-------------------------------------------------

QT       += core gui
QT += network
QT += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QQ
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    mylabel.cpp \
    mypushbutton.cpp \
    acctinfodetail.cpp \
    networkwindow.cpp \
    loginfailwindow.cpp \
    qqmainwindow.cpp \
    incomingmsgitem.cpp \
    centraltabwidget.cpp \
    msgtab.cpp \
    contacttab.cpp \
    contactitem.cpp \
    newfrienditem.cpp \
    classifyitem.cpp \
    listwidget.cpp \
    converstationwindow.cpp

HEADERS  += widget.h \
    mylabel.h \
    mypushbutton.h \
    acctinfodetail.h \
    networkwindow.h \
    loginfailwindow.h \
    qqmainwindow.h \
    incomingmsgitem.h \
    centraltabwidget.h \
    msgtab.h \
    contacttab.h \
    contactitem.h \
    newfrienditem.h \
    classifyitem.h \
    listwidget.h \
    converstationwindow.h

FORMS    += widget.ui

RESOURCES += \
    image.qrc

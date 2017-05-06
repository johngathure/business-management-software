#-------------------------------------------------
#
# Project created by QtCreator 2015-10-08T09:34:50
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AMAV_HARDWARE
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    addproductsframe.cpp \
    workerfunctions.cpp \
    tableformat.cpp \
    about.cpp

HEADERS  += mainwindow.h \
    login.h \
    addproductsframe.h \
    workerfunctions.h \
    global_var.h \
    tableformat.h \
    about.h

FORMS    += mainwindow.ui \
    login.ui \
    addproductsframe.ui \
    about.ui

RESOURCES += \
    app_resource.qrc

win32: RC_ICONS = boom.ico

CONFIG +=static



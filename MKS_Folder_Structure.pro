#-------------------------------------------------
#
# Project created by QtCreator 2013-10-27T00:34:39
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Integrity_Folder_Structure
TEMPLATE = app


SOURCES += main.cpp\
        maindialog.cpp \
    mylineeditclass.cpp \
    help.cpp

HEADERS  += maindialog.h \
    mylineeditclass.h \
    version.h \
    help.h

FORMS    += maindialog.ui \
    help.ui

RESOURCES +=

include(version.pro)

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

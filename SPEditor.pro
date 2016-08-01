#-------------------------------------------------
#
# Project created by QtCreator 2016-07-16T19:42:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SPEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qnumbertablewidgetitem.cpp \
    qflatcombobox.cpp

HEADERS  += mainwindow.h \
    qnumbertablewidgetitem.h \
    qflatcombobox.h

FORMS    += mainwindow.ui

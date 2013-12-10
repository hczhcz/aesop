#-------------------------------------------------
#
# Project created by QtCreator 2013-12-09T00:29:03
#
#-------------------------------------------------

QT += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = calc
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
    mainwindow.cpp \
    mmlwidget/qtmmlwidget.cpp \
    opparser/opparser.cpp \
    opparser/opcalcnear.cpp \
    opparser/opcalc.cpp

HEADERS  += mainwindow.h \
    mmlwidget/QtMmlWidget \
    mmlwidget/qtmmlwidget.h \
    mmlwidget/QtMmlDocument \
    opparser/opparser.hpp \
    opparser/opcalcrule.hpp \
    opparser/opcalcnear.hpp \
    opparser/opcalc.hpp

FORMS    += mainwindow.ui

OTHER_FILES += \
    mmlwidget/qtmmlwidget.pri

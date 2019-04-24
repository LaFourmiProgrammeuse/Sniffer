QT += core
QT -= gui

TARGET = Snifffer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

QMAKE_CXXFLAGS += -std=c++11


unix:!macx: LIBS += -L$$_PRO_FILE_PWD_/libtins/lib/linux -ltins

INCLUDEPATH += $$_PRO_FILE_PWD_/libtins/include
DEPENDPATH += $$_PRO_FILE_PWD_/libtins/include

unix:!macx: LIBS += -L$$_PRO_FILE_PWD_/boost/lib/linux/ -lboost_regex

INCLUDEPATH += $$PWD/'_PRO_FILE_PWD_/boost/include'
DEPENDPATH += $$PWD/'_PRO_FILE_PWD_/boost/include'

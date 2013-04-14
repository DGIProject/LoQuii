#-------------------------------------------------
#
# Project created by QtCreator 2012-04-22T18:02:15
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql
QT += core gui webkit network

TARGET = macubaGraph
TEMPLATE = app


SOURCES += main.cpp\
        fenprincipal.cpp \
    fenmacumba.cpp \
    apropos.cpp \
    fenoptions.cpp

HEADERS  += fenprincipal.h \
    fenmacumba.h \
    apropos.h \
    fenoptions.h

FORMS    += fenprincipal.ui \
    fenmacumba.ui \
    apropos.ui \
    fenoptions.ui

TRANSLATIONS = loquii_en.ts loquii_es.ts

INCLUDEPATH += $$PWD/../../../../usr/include
DEPENDPATH += $$PWD/../../../../usr/include


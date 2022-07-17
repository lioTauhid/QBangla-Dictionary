#-------------------------------------------------
#
# Project created by QtCreator 2018-02-17T07:11:32
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BijoyEkush
TEMPLATE = app


SOURCES += main.cpp\
        bijoyekush_bangla_dictionary.cpp \
    about.cpp \
    help.cpp

HEADERS  += bijoyekush_bangla_dictionary.h \
    about.h \
    help.h

FORMS    += bijoyekush_bangla_dictionary.ui \
    about.ui \
    help.ui

RESOURCES += \
    myf.qrc

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = Gaph
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS



INCLUDEPATH+=/home/diman-pro2/INCLUDE/
LIBS+=/home/diman-pro2/INCLUDE/libas.so
SOURCES += \
        main.cpp \
        compiler.cpp \
    draw.cpp \
    editer.cpp

HEADERS += compiler.h \
        draw.h \
    editer.h 

FORMS += \
        draw.ui \
    editer.ui

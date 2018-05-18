#-------------------------------------------------
#
# Project created by QtCreator 2018-05-16T10:09:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HairTest
TEMPLATE = app

PRECOMPILED_HEADER = src/stable.h


SOURCES += src/main.cpp \
    src/hair/hairnode.cpp \
    src/hair/hairstrand.cpp \
    src/mainwindow.cpp \
    src/model.cpp \
    src/openglscene.cpp \
    src/hair/hair.cpp \
    src/sphere.cpp \
    src/hair/scalp.cpp

HEADERS  += \
    src/hair/hairnode.h \
    src/hair/hairstrand.h \
    src/mainwindow.h \
    src/model.h \
    src/openglscene.h \
    src/stable.h \
    src/ui_mainwindow.h \
    src/hair/hair.h \
    src/sphere.h \
    src/hair/scalp.h

FORMS += \
    src/mainwindow.ui

INCLUDEPATH += $$PWD\src\
INCLUDEPATH += $$PWD\src\hair\

DISTFILES += \
    shader/fshader.glsl \
    shader/vshader.glsl



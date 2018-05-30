#-------------------------------------------------
#
# Project created by QtCreator 2018-05-16T10:09:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HairTest
TEMPLATE = app

UI_DIR= ui

PRECOMPILED_HEADER = src/stable.h


SOURCES += src/main.cpp \
    src/hair/hairnode.cpp \
    src/hair/hairstrand.cpp \
    src/mainwindow.cpp \
    src/model.cpp \
    src/openglscene.cpp \
    src/sphere.cpp \
    src/env.cpp \
    src/hair/hairmodel.cpp \
    src/line.cpp \
    src/bcurve.cpp

HEADERS  += \
    src/hair/hairnode.h \
    src/hair/hairstrand.h \
    src/mainwindow.h \
    src/model.h \
    src/openglscene.h \
    src/stable.h \
    src/sphere.h \
    src/env.h \
    src/hair/hairmodel.h \
    src/line.h \
    src/bcurve.h

FORMS += \
    ui/mainwindow.ui


INCLUDEPATH += $$PWD\src\
INCLUDEPATH += $$PWD\src\hair\
INCLUDEPATH += $$PWD\ui\

DISTFILES += \
    shader/fshader.glsl \
    shader/vshader.glsl



#-------------------------------------------------
#
# Project created by QtCreator 2019-08-15T22:19:32
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qsonar
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    src/main.cpp\
    src/mainwindow.cpp \
    src/qsb_serailreader.cpp \
    src/qsb_object.cpp \
    src/qsb_serialreadermock.cpp \
    src/qsb_sonar.cpp

HEADERS  += \
    src/mainwindow.h \
    src/qsb_serailreader.h \
    src/qsb_object.h \
    src/qsb_serialreadermock.h \
    src/qsb_sonar.h \
    src/qsb_sonardata.h

FORMS    += \
    res/mainwindow.ui

DISTFILES += \
    sonar/sonar.ino \
    README.md

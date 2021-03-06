#-------------------------------------------------
#
# Project created by QtCreator 2019-11-11T20:18:03
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Elka-Drive-Desktop
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    ../Backend \
    ../qamqp/src

win32 {
    LIBS += -L../Backend/release \
        -lBackend \
        -L../qamqp/src \
        -lqamqp
}
unix {
    LIBS += -L../Backend \
        -lBackend \
        -L../qamqp/src \
        -lqamqp
}

SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp

HEADERS += \
    include/mainwindow.h \
    include/rabbitmqsecrets.h

FORMS += \
    forms/mainwindow.ui

RESOURCES += \
    resources/res.qrc

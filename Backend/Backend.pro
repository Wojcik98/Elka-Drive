QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Backend
TEMPLATE = lib
DEFINES += BACKEND_LIBRARY

CONFIG += staticlib
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/groupsettingsdialog.cpp \
    src/registerdialog.cpp \
    src/response.cpp \
    src/apibridge.cpp \
    src/controller.cpp \
    src/logindialog.cpp \
    src/model.cpp \
    src/user.cpp \
    src/view.cpp \
    src/webbridge.cpp

HEADERS += \
    include/apibridge.h \
    include/controller.h \
    include/groupsettingsdialog.h \
    include/logindialog.h \
    include/model.h \
    include/registerdialog.h \
    include/response.h \
    include/user.h \
    include/view.h \
    include/webbridge.h

RESOURCES += \
    resources/res.qrc

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

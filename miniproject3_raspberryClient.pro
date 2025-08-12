QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    mycore/backend.cpp \
    ui/checkui.cpp \
    ui/depositui.cpp \
    ui/finishui.cpp \
    senlib/light.cpp \
    mycore/main.cpp \
    ui/mainmenuui.cpp \
    ui/mainwindow.cpp \
    senlib/mylineedit.cpp \
    senlib/piezo.cpp \
    ui/rfidui.cpp \
    ui/sendui.cpp \
    mycore/sensorworker.cpp \
    senlib/CommSPI.cpp \
    senlib/MFRC522.cpp \
    mycore/rf522worker.cpp \
    mycore/webclient.cpp

HEADERS += \
    mycore/backend.h \
    ui/checkui.h \
    ui/depositui.h \
    ui/finishui.h \
    senlib/light.h \
    ui/mainmenuui.h \
    ui/mainwindow.h \
    senlib/mylineedit.h \
    senlib/piezo.h \
    ui/rfidui.h \
    ui/sendui.h \
    mycore/sensorworker.h \
    senlib/Comm.h \
    senlib/MFRC522.h \
    mycore/rf522worker.h \
    mycore/webclient.h

FORMS += \
    ui/checkui.ui \
    ui/depositui.ui \
    ui/finishui.ui \
    ui/mainmenuui.ui \
    ui/mainwindow.ui \
    ui/rfidui.ui \
    ui/sendui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# using library
LIBS += -lgpiod

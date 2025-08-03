QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    depositui.cpp \
    light.cpp \
    main.cpp \
    mainmenuui.cpp \
    mainwindow.cpp \
    piezo.cpp \
    rfidui.cpp \
    sensorworker.cpp \
    CommSPI.cpp \
    MFRC522.cpp \
    rf522worker.cpp

HEADERS += \
    depositui.h \
    light.h \
    mainmenuui.h \
    mainwindow.h \
    piezo.h \
    rfidui.h \
    sensorworker.h \
    Comm.h \
    MFRC522.h \
    rf522worker.h

FORMS += \
    depositui.ui \
    mainmenuui.ui \
    mainwindow.ui \
    rfidui.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# using library
LIBS += -lgpiod

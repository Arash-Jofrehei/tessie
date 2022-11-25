QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = tessie

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(PI) {
   DEFINES += PI
#   LIBS += -lwiringPi
} else {

}

SOURCES += \
    CANmessage.cc \
    MainWindow.cpp \
    TECDisplay.cpp \
    canFrame.cc \
    driveHardware.cc \
    tLog.cc \
    tessie.cpp

HEADERS += \
    CANmessage.hh \
    MainWindow.h \
    TECData.hh \
    TECDisplay.h \
    TECRegister.hh \
    canFrame.hh \
    driveHardware.hh \
    tLog.hh

FORMS += \
    MainWindow.ui \
    TECDisplay.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

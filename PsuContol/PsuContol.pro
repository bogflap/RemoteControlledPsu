QT -= gui
QT += serialport

TEMPLATE = lib
DEFINES += PSUCONTOL_LIBRARY

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Params_72_2710.cpp \
    psucontrol.cpp

HEADERS += \
    Params_72_2710.h \
    PsuContol_global.h \
    psucontrol.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

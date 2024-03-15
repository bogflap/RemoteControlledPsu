QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    lcdcontroller.cpp \
    lcdfloattodigits.cpp \
    main.cpp \
    mainwindow.cpp \
    psuThread.cpp

HEADERS += \
    lcdcontroller.h \
    lcdfloattodigits.h \
    mainwindow.h \
    psuThread.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources/resources.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PsuContol/release/ -lPsuContol
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PsuContol/debug/ -lPsuContol
else:unix: LIBS += -L$$OUT_PWD/../PsuContol/ -lPsuContol

INCLUDEPATH += $$PWD/../PsuContol
DEPENDPATH += $$PWD/../PsuContol

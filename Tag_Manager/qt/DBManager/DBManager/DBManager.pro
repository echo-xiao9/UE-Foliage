QT       += core gui
QT       += sql
QT       += openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    chooseexistdb.cpp \
    choosenewtagtype.cpp \
    dboperator.cpp \
    main.cpp \
    mainwindow.cpp \
    plant.cpp \
    plant_browser.cpp \
    test.cpp

HEADERS += \
    chooseexistdb.h \
    choosenewtagtype.h \
    dboperator.h \
    mainwindow.h \
    plant.h \
    plant_browser.h \
    test.h

FORMS += \
    chooseexistdb.ui \
    choosenewtagtype.ui \
    mainwindow.ui \
    plant_browser.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc

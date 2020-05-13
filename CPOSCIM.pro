QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QT += charts
QT += axcontainer
win32:QT += winextras

VERSION = 1.1.0
QMAKE_TARGET_DESCRIPTION = This is a calculation program of stray current in Metro
QMAKE_TARGET_PRODUCT = Calculation program of stray current in Metro
QMAKE_TARGET_COPYRIGHT = buy ZK

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
    calculate.cpp \
    chartview.cpp \
    dialogwithonebutn.cpp \
    dialogwithtwobutn.cpp \
    lineeditqrealdelegate.cpp \
    localtip.cpp \
    main.cpp \
    mainwindow.cpp \
    morechartswidget.cpp \
    mousechart.cpp \
    powertablemodel.cpp \
    queryresultthread.cpp \
    saveresultthread.cpp \
    stopthread.cpp \
    tip.cpp \
    traintablemodel.cpp

HEADERS += \
    calculate.h \
    chartview.h \
    dialogwithonebutn.h \
    dialogwithtwobutn.h \
    lineeditqrealdelegate.h \
    localtip.h \
    mainwindow.h \
    morechartswidget.h \
    mousechart.h \
    powertablemodel.h \
    queryresultthread.h \
    saveresultthread.h \
    stopthread.h \
    tip.h \
    traintablemodel.h

FORMS += \
    dialogwithonebutn.ui \
    dialogwithtwobutn.ui \
    mainwindow.ui \
    morechartswidget.ui

RC_ICONS = 6.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    allimages.qrc

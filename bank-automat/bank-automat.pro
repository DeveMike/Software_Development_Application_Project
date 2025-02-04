QT       += core gui
QT +=network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    balance.cpp \
    customerdata.cpp \
    environment.cpp \
    login.cpp \
    main.cpp \
    mainmenu.cpp \
    transaction.cpp \
    transfer.cpp \
    withdraw.cpp

HEADERS += \
    balance.h \
    customerdata.h \
    environment.h \
    login.h \
    mainmenu.h \
    transaction.h \
    transfer.h \
    withdraw.h

FORMS += \
    balance.ui \
    customerdata.ui \
    login.ui \
    mainmenu.ui \
    transaction.ui \
    transfer.ui \
    withdraw.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

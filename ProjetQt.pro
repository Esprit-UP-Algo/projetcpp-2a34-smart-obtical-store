QT       += core gui sql charts network \
    quick

QT += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    connection.cpp \
    dialog.cpp \
    emailsender.cpp \
    employee.cpp \
    main.cpp \
    mainwindow.cpp \
    message.cpp \
    messagerie.cpp

HEADERS += \
    connection.h \
    dialog.h \
    emailsender.h \
    employee.h \
    mainwindow.h \
    message.h \
    messagerie.h

FORMS += \
    dialog.ui \
    mainwindow.ui \
    messagerie.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

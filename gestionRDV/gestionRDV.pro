QT += core gui sql widgets printsupport

CONFIG += c++17

TARGET = GestionRDV
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    rdv.cpp

HEADERS += \
    mainwindow.h \
    connection.h \
    rdv.h

FORMS += \
    mainwindow.ui

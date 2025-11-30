QT += core gui sql widgets printsupport


CONFIG += c++17

TARGET = GestionRDV
TEMPLATE = app

SOURCES += \
    client.cpp \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    produit.cpp \
    rdv.cpp

HEADERS += \
    client.h \
    mainwindow.h \
    connection.h \
    produit.h \
    rdv.h

FORMS += \
    mainwindow.ui

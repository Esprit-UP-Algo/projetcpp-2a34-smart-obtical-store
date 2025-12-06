QT += core gui sql widgets printsupport charts

CONFIG += c++17

TARGET = GestionRDV
TEMPLATE = app

SOURCES += \
    client.cpp \
    fournisseur.cpp \
    historique.cpp \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    produit.cpp \
    rdv.cpp

HEADERS += \
    client.h \
    fournisseur.h \
    historique.h \
    mainwindow.h \
    connection.h \
    produit.h \
    rdv.h

FORMS += \
    mainwindow.ui


QT       += core gui sql printsupport charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Atelier_Connexion
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
    client.cpp \
    main.cpp \
    mainwindow.cpp \
    connection.cpp \
    qrcodegenerator.cpp

HEADERS += \
    qrcodegenerator.h \
    client.h \
    mainwindow.h \
    connection.h

FORMS += \
    mainwindow.ui

# Nettoyer les doublons - garder seulement cette ligne pour QT
QT += core gui sql printsupport charts widgets

# Supprimer cette ligne car la bibliothèque n'est pas installée
# LIBS += -lqrcodegen

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

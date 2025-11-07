#include "mainwindow.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Connection c;
    bool test = c.createconnect();

    if (test)
    {
        QMessageBox::information(nullptr, QObject::tr("Base de données ouverte"),
                                 QObject::tr("Connexion réussie.\nCliquez sur OK pour continuer."));
        MainWindow w;
        w.show();
        return a.exec();
    }
    else
    {
        QMessageBox::critical(nullptr, QObject::tr("Base de données non ouverte"),
                              QObject::tr("Échec de la connexion.\nCliquez sur OK pour quitter."));
        return -1;
    }
}


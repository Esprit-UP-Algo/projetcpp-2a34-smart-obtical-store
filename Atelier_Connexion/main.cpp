#include <QApplication>
#include "mainwindow.h"
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Singleton connection
    Connection& c = Connection::getInstance();
    if(!c.createConnection()){
        qDebug() << "Erreur connexion base";
        return -1;
    }

    MainWindow w;
    w.show();
    return a.exec();
}

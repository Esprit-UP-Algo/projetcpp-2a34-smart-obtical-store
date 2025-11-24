#include "connection.h"
#include <QSqlError>
#include <QMessageBox>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("base");        // Nom de la source ODBC
    db.setUserName("aziz");
    db.setPassword("0000");

    if (db.open()) {
        return true;
    } else {
        QMessageBox::critical(nullptr, "Erreur de connexion",
                              "Impossible de se connecter à la base :\n" + db.lastError().text());
        return false;
    }
}

void Connection::closeconnect()
{
    if (db.isOpen())
        db.close();
}

Connection::~Connection()
{
    closeconnect();
}

#include "connection.h"
#include <QDebug>
#include <QSqlError>

// Singleton instance
Connection& Connection::getInstance()
{
    static Connection instance;
    return instance;
}

// Constructeur privé
Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
}

// Destructeur
Connection::~Connection()
{
    if (db.isOpen())
        db.close();
}

// Établir la connexion Oracle via ODBC
bool Connection::createConnection()
{
    db.setDatabaseName("Projet_2A2526"); // Nom du DSN
    db.setUserName("rahma");
    db.setPassword("rahma123");

    if (db.open()) {
        qDebug() << "✅ Connexion Oracle établie";
        return true;
    } else {
        qDebug() << "❌ Échec connexion Oracle :" << db.lastError().text();
        return false;
    }
}


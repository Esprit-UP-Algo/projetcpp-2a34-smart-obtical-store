#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Connection::Connection()
{
}

bool Connection::createconnect()
{
    qDebug() << "=== Starting Database Connection ===";
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("SYSTEM");
    db.setPassword("esprit18");

    if (db.open())
    {
        test = true;
        qDebug() << "✓✓✓ DATABASE CONNECTED SUCCESSFULLY! ✓✓✓";
        qDebug() << "=== Connection Complete ===\n";
    }
    else
    {
        qDebug() << "✗✗✗ DATABASE CONNECTION FAILED! ✗✗✗";
        qDebug() << "Error:" << db.lastError().text();
    }

    return test;
}

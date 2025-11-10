#include "connection.h"

Connection::Connection() // attention à la majuscule ici
{
}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("smartopticalstore");
    db.setUserName("maycem");
    db.setPassword("1234");

    if (db.open())
        test = true;

    return test;
}

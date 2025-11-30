#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QDebug>

class Connection
{
public:
    // Singleton instance
    static Connection& getInstance();

    // Établir la connexion
    bool createConnection();

    QSqlDatabase& getDatabase() { return db; }

private:
    QSqlDatabase db;

    // Constructeur privé pour singleton
    Connection();

    // Empêcher copie et assignation
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    // Destructeur
    ~Connection();
};

#endif // CONNECTION_H

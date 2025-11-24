#ifndef CONNECTION_H
#define CONNECTION_H

#include <QSqlDatabase>

class Connection
{
private:
    QSqlDatabase db;

public:
    Connection();
    ~Connection();
    bool createconnect();
    void closeconnect();
};

#endif // CONNECTION_H

#ifndef FIND_H
#define FIND_H

#include <QSqlQueryModel>
#include <QTableWidget>          // ←←←← THIS LINE WAS MISSING !!!!

class FindCommande
{
public:
    static QSqlQueryModel* rechercher(const QString &type, const QString &valeur);
    static void chargerTableau(QTableWidget *table);   // Now OK
};

#endif // FIND_H

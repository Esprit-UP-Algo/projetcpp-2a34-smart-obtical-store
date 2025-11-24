#ifndef ADD_H
#define ADD_H

#include "commande.h"
#include <QSqlQuery>

class AddCommande
{
public:
    static bool ajouter(const Commande &c);
};

#endif // ADD_H

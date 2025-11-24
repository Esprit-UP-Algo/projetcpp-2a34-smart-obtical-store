#ifndef COMMANDE_H
#define COMMANDE_H

#include <QString>
#include <QDate>

struct Commande {
    int ref_c;
    int id_c;
    QString mode_paiement;
    QDate date_commande;
    double montant_total;
    QString etat;
    double montant_a_payer;
};

#endif // COMMANDE_H

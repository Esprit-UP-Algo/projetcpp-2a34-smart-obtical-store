#include "add.h"
#include <QMessageBox>
#include <QSqlError>

bool AddCommande::ajouter(const Commande &c)
{
    QSqlQuery query;
    query.prepare("INSERT INTO commande (ref_c, ID_c, ID_E, mode_payaement, date_commande, montant_totale, etat, montant_a_payer) "
                  "VALUES (:ref, :idc, 1, :mode, TO_DATE(:date, 'YYYY-MM-DD'), :total, :etat, :apayer)");

    query.bindValue(":ref", c.ref_c);
    query.bindValue(":idc", c.id_c);
    query.bindValue(":mode", c.mode_paiement);
    query.bindValue(":date", c.date_commande.toString("yyyy-MM-dd"));
    query.bindValue(":total", c.montant_total);
    query.bindValue(":etat", c.etat);
    query.bindValue(":apayer", c.montant_a_payer);

    if (query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Commande ajoutée avec succès !");
        return true;
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'ajout :\n" + query.lastError().text());
        return false;
    }
}

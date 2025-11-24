#include "update.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>
bool UpdateCommande::modifier(const Commande &c)
{
    QSqlQuery query;

    // D'abord vérifier si la référence existe
    query.prepare("SELECT ref_c FROM commande WHERE ref_c = :ref");
    query.bindValue(":ref", c.ref_c);
    if (!query.exec() || !query.next()) {
        QMessageBox::warning(nullptr, "Introuvable", "Aucune commande avec la référence " + QString::number(c.ref_c) + " n'existe.");
        return false;
    }

    // Mise à jour
    query.prepare("UPDATE commande SET "
                  "ID_c = :idc, "
                  "mode_payaement = :mode, "
                  "date_commande = TO_DATE(:date, 'YYYY-MM-DD'), "
                  "montant_totale = :total, "
                  "etat = :etat, "
                  "montant_a_payer = :apayer "
                  "WHERE ref_c = :ref");

    query.bindValue(":ref", c.ref_c);
    query.bindValue(":idc", c.id_c);
    query.bindValue(":mode", c.mode_paiement);
    query.bindValue(":date", c.date_commande.toString("yyyy-MM-dd"));
    query.bindValue(":total", c.montant_total);
    query.bindValue(":etat", c.etat);
    query.bindValue(":apayer", c.montant_a_payer);

    if (query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Commande modifiée avec succès !");
        return true;
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la modification :\n" + query.lastError().text());
        return false;
    }
}

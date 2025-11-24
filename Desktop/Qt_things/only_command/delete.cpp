#include "delete.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>    // ← THIS LINE FIXES EVERYTHING!s
bool DeleteCommande::supprimer(int ref_c)
{
    QSqlQuery query;

    // First check if exists
    query.prepare("SELECT ref_c FROM commande WHERE ref_c = :ref");
    query.bindValue(":ref", ref_c);
    if (!query.exec() || !query.next()) {
        QMessageBox::warning(nullptr, "Introuvable", "Cette commande n'existe pas !");
        return false;
    }

    // Delete it
    query.prepare("DELETE FROM commande WHERE ref_c = :ref");
    query.bindValue(":ref", ref_c);

    if (query.exec()) {
        QMessageBox::information(nullptr, "Supprimé", "Commande supprimée avec succès !");
        return true;
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Échec de la suppression :\n" + query.lastError().text());
        return false;
    }
}

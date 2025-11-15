#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Fournisseur
{
public:
    Fournisseur();
    Fournisseur(int id, QString nom_societe, QString adresse,
                QString email, QString telephone, QString specialite);

    // CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int id);

    // Affichage
    QSqlQueryModel* afficher();

    // Recherche
    QSqlQueryModel* rechercher(QString value);

    // Tri
    QSqlQueryModel* trier(QString critere, QString ordre);

    // Statistique
    QSqlQueryModel* statistique();

private:
    int id;
    QString nom_societe, adresse, email, telephone, specialite;
};

#endif // FOURNISSEUR_H

#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

class Produit
{
public:
    Produit();
    // Constructeur SANS date (ancien)
    Produit(int, QString, QString, double, int);
    // NOUVEAU : Constructeur AVEC date (obligatoire maintenant)
    Produit(int ref, QString cat, QString marque, double prix, int qte, QDate date);

    bool ajouter();
    bool modifier();
    bool annuler(int ref);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(QString critere);
    QSqlQueryModel* trier(QString critere, QString ordre);


private:
    int REF_P, QUANTITE;
    QString CATEGORIE, MARQUE;
    double PRIX;
    QDate DATE_P;   // Ajouté
};

#endif // PRODUIT_H


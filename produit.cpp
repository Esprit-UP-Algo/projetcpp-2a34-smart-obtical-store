#include "produit.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QVariant>
#include <QPdfWriter>
#include <QPainter>
#include <QFont>
#include <QPageSize>
#include <QDebug>

Produit::Produit() {}

Produit::Produit(int ref, QString cat, QString marque, double prix, int qte)
{
    REF_P = ref;
    CATEGORIE = cat;
    MARQUE = marque;
    PRIX = prix;
    QUANTITE = qte;
    DATE_P = QDate::currentDate(); // date du jour par défaut
}

// NOUVEAU CONSTRUCTEUR
Produit::Produit(int ref, QString cat, QString marque, double prix, int qte, QDate date)
{
    REF_P = ref;
    CATEGORIE = cat;
    MARQUE = marque;
    PRIX = prix;
    QUANTITE = qte;
    DATE_P = date;   // ← la date qu’on passe depuis l’interface
}

bool Produit::ajouter()
{
    QSqlQuery qry;
    qry.prepare("INSERT INTO PRODUIT (REF_P, CATEGORIE, MARQUE, PRIX, QUANTITE, DATE_P) "
                "VALUES (:ref, :cat, :marque, :prix, :qte, :date)");
    qry.bindValue(":ref", REF_P);
    qry.bindValue(":cat", CATEGORIE);
    qry.bindValue(":marque", MARQUE);
    qry.bindValue(":prix", PRIX);
    qry.bindValue(":qte", QUANTITE);
    qry.bindValue(":date", DATE_P);   // ← important
    return qry.exec();
}

bool Produit::modifier()
{
    QSqlQuery qry;
    qry.prepare("UPDATE PRODUIT SET CATEGORIE=:cat, MARQUE=:marque, PRIX=:prix, QUANTITE=:qte WHERE REF_P=:ref");
    qry.bindValue(":ref", REF_P);
    qry.bindValue(":cat", CATEGORIE);
    qry.bindValue(":marque", MARQUE);
    qry.bindValue(":prix", PRIX);
    qry.bindValue(":qte", QUANTITE);
    return qry.exec();
}

// Le reste de ton code (annuler, afficher, rechercher, trier, exporterPDF) reste inchangé

bool Produit::annuler(int ref)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PRODUIT WHERE REF_P=:ref");
    query.bindValue(":ref", ref);
    return query.exec();
}

QSqlQueryModel* Produit::rechercher(QString critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT * FROM PRODUIT WHERE "
                       "REF_P LIKE '%" + critere + "%' OR "
                                   "CATEGORIE LIKE '%" + critere + "%' OR "
                                   "MARQUE LIKE '%" + critere + "%'";
    model->setQuery(queryStr);
    return model;
}

QSqlQueryModel* Produit::trier(QString critere, QString ordre)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    // Validation des paramètres
    if (ordre != "ASC" && ordre != "DESC") {
        ordre = "ASC";
    }

    QString queryStr = "SELECT REF_P, MARQUE, CATEGORIE, PRIX, QUANTITE, DATE_P FROM PRODUIT ORDER BY " + critere + " " + ordre;
    model->setQuery(queryStr);
    return model;
}





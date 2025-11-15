#include "fournisseur.h"

Fournisseur::Fournisseur()
{
    id = 0;
}

Fournisseur::Fournisseur(int id, QString nom_societe, QString adresse,
                         QString email, QString telephone, QString specialite)
{
    this->id = id;
    this->nom_societe = nom_societe;
    this->adresse = adresse;
    this->email = email;
    this->telephone = telephone;
    this->specialite = specialite;
}

// ------------------ AJOUT --------------------
bool Fournisseur::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO fournisseur (id, nom_societe, adresse, email, telephone, specialite) "
                  "VALUES (:id, :nom, :adresse, :email, :telephone, :specialite)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom_societe);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":specialite", specialite);

    return query.exec();
}

// ------------------ SUPPRESSION --------------------
bool Fournisseur::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM fournisseur WHERE id = :id");
    query.bindValue(":id", id);

    return query.exec();
}

// ------------------ MODIFICATION --------------------
bool Fournisseur::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE fournisseur SET nom_societe = :nom, adresse = :adresse, "
                  "email = :email, telephone = :telephone, specialite = :specialite "
                  "WHERE id = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom_societe);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":specialite", specialite);

    return query.exec();
}

// ------------------ AFFICHAGE --------------------
QSqlQueryModel* Fournisseur::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM fournisseur");
    return model;
}

// ------------------ RECHERCHE --------------------
QSqlQueryModel* Fournisseur::rechercher(QString value)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM fournisseur WHERE "
                    "LOWER(nom_societe) LIKE LOWER('%" + value + "%') OR "
                              "LOWER(specialite) LIKE LOWER('%" + value + "%') OR "
                              "CAST(id AS VARCHAR(20)) LIKE '%" + value + "%'");
    return model;
}

// ------------------ TRI --------------------
QSqlQueryModel* Fournisseur::trier(QString critere, QString ordre)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString query = "SELECT * FROM fournisseur ORDER BY " + critere + " " + ordre;
    model->setQuery(query);
    return model;
}

// ------------------ STATISTIQUE --------------------
QSqlQueryModel* Fournisseur::statistique()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery("SELECT specialite, COUNT(*) "
                    "FROM fournisseur "
                    "GROUP BY specialite "
                    "ORDER BY COUNT(*) DESC");

    return model;
}

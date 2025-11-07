#include "fournisseur.h"
#include <QSqlQuery>
#include <QVariant>

Fournisseur::Fournisseur()
{
    id = 0;
    nom_societe = adresse = email = telephone = specialite = "";
}

Fournisseur::Fournisseur(int id, QString nom, QString adr, QString em, QString tel, QString spec)
{
    this->id = id;
    nom_societe = nom;
    adresse = adr;
    email = em;
    telephone = tel;
    specialite = spec;
}

// Getters
int Fournisseur::getId() { return id; }
QString Fournisseur::getNomSociete() { return nom_societe; }
QString Fournisseur::getAdresse() { return adresse; }
QString Fournisseur::getEmail() { return email; }
QString Fournisseur::getTelephone() { return telephone; }
QString Fournisseur::getSpecialite() { return specialite; }

// Setters
void Fournisseur::setId(int i) { id = i; }
void Fournisseur::setNomSociete(QString n) { nom_societe = n; }
void Fournisseur::setAdresse(QString a) { adresse = a; }
void Fournisseur::setEmail(QString e) { email = e; }
void Fournisseur::setTelephone(QString t) { telephone = t; }
void Fournisseur::setSpecialite(QString s) { specialite = s; }

// CRUD
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

QSqlQueryModel *Fournisseur::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM fournisseur");
    return model;
}

bool Fournisseur::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM fournisseur WHERE id = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Fournisseur::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE fournisseur SET nom_societe=:nom, adresse=:adresse, email=:email, "
                  "telephone=:telephone, specialite=:specialite WHERE id=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom_societe);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":specialite", specialite);

    return query.exec();
}

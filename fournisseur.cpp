#include "fournisseur.h"
#include <QSqlQuery>
#include <QVariant>

Fournisseur::Fournisseur()
    : id(0), nom_societe(""), adresse(""), email(""), telephone(""), specialite("") {}

Fournisseur::Fournisseur(int id, QString nom, QString adr, QString em, QString tel, QString spec)
    : id(id), nom_societe(nom), adresse(adr), email(em), telephone(tel), specialite(spec) {}

int Fournisseur::getId() const { return id; }
QString Fournisseur::getNomSociete() const { return nom_societe; }
QString Fournisseur::getAdresse() const { return adresse; }
QString Fournisseur::getEmail() const { return email; }
QString Fournisseur::getTelephone() const { return telephone; }
QString Fournisseur::getSpecialite() const { return specialite; }

void Fournisseur::setId(int i) { id = i; }
void Fournisseur::setNomSociete(const QString &n) { nom_societe = n; }
void Fournisseur::setAdresse(const QString &a) { adresse = a; }
void Fournisseur::setEmail(const QString &e) { email = e; }
void Fournisseur::setTelephone(const QString &t) { telephone = t; }
void Fournisseur::setSpecialite(const QString &s) { specialite = s; }

bool Fournisseur::ajouter() const
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

QSqlQueryModel *Fournisseur::afficher(const QString &filter, const QString &orderBy) const
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QString base = "SELECT id AS \"ID\", nom_societe AS \"Nom société\", adresse AS \"Adresse\", email AS \"Email\", telephone AS \"Téléphone\", specialite AS \"Spécialité\" FROM fournisseur";
    QString queryStr = base;
    if (!filter.isEmpty()) {
        queryStr += " WHERE " + filter;
    }
    if (!orderBy.isEmpty()) {
        queryStr += " ORDER BY " + orderBy;
    }
    model->setQuery(queryStr);
    return model;
}

bool Fournisseur::supprimer(int idparam) const
{
    QSqlQuery query;
    query.prepare("DELETE FROM fournisseur WHERE id = :id");
    query.bindValue(":id", idparam);
    return query.exec();
}

bool Fournisseur::modifier() const
{
    QSqlQuery query;
    query.prepare("UPDATE fournisseur SET nom_societe=:nom, adresse=:adresse, email=:email, telephone=:telephone, specialite=:specialite WHERE id=:id");
    query.bindValue(":id", id);
    query.bindValue(":nom", nom_societe);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":specialite", specialite);
    return query.exec();
}

QSqlQueryModel* Fournisseur::chercher(const QString &term)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    // recherche sur id (exact) ou sur nom/nom_societe/spécialité (LIKE)
    query.prepare("SELECT id AS \"ID\", nom_societe AS \"Nom société\", adresse AS \"Adresse\", email AS \"Email\", telephone AS \"Téléphone\", specialite AS \"Spécialité\" "
                  "FROM fournisseur WHERE CAST(id AS VARCHAR(100)) LIKE :t OR lower(nom_societe) LIKE :t OR lower(specialite) LIKE :t");
    QString pattern = "%" + term.toLower() + "%";
    query.bindValue(":t", pattern);
    query.exec();
    model->setQuery(query);
    return model;
}

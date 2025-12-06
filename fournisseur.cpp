#include "fournisseur.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>  // Ajoutez cette ligne

// Constructors
Fournisseur::Fournisseur() : id(0) {}

Fournisseur::Fournisseur(int id, const QString &nom, const QString &adresse,
                         const QString &email, const QString &telephone, const QString &specialite)
    : id(id), nom(nom), adresse(adresse), email(email), telephone(telephone), specialite(specialite)
{}

// Getters
int Fournisseur::getId() const { return id; }
QString Fournisseur::getNom() const { return nom; }
QString Fournisseur::getAdresse() const { return adresse; }
QString Fournisseur::getEmail() const { return email; }
QString Fournisseur::getTelephone() const { return telephone; }
QString Fournisseur::getSpecialite() const { return specialite; }

// Setters
void Fournisseur::setId(int i) { id = i; }
void Fournisseur::setNom(const QString &n) { nom = n; }
void Fournisseur::setAdresse(const QString &a) { adresse = a; }
void Fournisseur::setEmail(const QString &e) { email = e; }
void Fournisseur::setTelephone(const QString &t) { telephone = t; }
void Fournisseur::setSpecialite(const QString &s) { specialite = s; }

// ------------------ AJOUT --------------------
bool Fournisseur::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO FOURNISSEUR (ID_F, NOM_SOCIETE, ADRESSE_F, MAIL_F, TEL_F, SPECIALITE) "
                  "VALUES (:id, :nom, :adresse, :email, :telephone, :specialite)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":specialite", specialite);

    if (!query.exec()) {
        qDebug() << "Erreur d'ajout fournisseur:" << query.lastError().text();
        return false;
    }
    return true;
}

// ------------------ MODIFICATION --------------------
bool Fournisseur::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE FOURNISSEUR SET NOM_SOCIETE = :nom, ADRESSE_F = :adresse, "
                  "MAIL_F = :email, TEL_F = :telephone, SPECIALITE = :specialite "
                  "WHERE ID_F = :id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":specialite", specialite);

    if (!query.exec()) {
        qDebug() << "Erreur de modification fournisseur:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Fournisseur::annuler(int id)
{
    return supprimer(id);  // Utilise la même fonction
}

bool Fournisseur::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM FOURNISSEUR WHERE ID_F = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur de suppression fournisseur:" << query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Fournisseur::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr = "SELECT ID_F, NOM_SOCIETE, ADRESSE_F, MAIL_F, TEL_F, SPECIALITE "
                       "FROM FOURNISSEUR ORDER BY ID_F";

    model->setQuery(queryStr);

    // Vérifiez s'il y a une erreur
    if (model->lastError().isValid()) {
        qDebug() << "Erreur dans afficher():" << model->lastError().text();
    } else {
        qDebug() << "Nombre de fournisseurs trouvés:" << model->rowCount();
    }

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom Société");
    model->setHeaderData(2, Qt::Horizontal, "Adresse");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Téléphone");
    model->setHeaderData(5, Qt::Horizontal, "Spécialité");

    return model;
}

QSqlQueryModel* Fournisseur::rechercher(const QString &critere)
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QString sql = QString("SELECT ID_F, NOM_SOCIETE, ADRESSE_F, MAIL_F, TEL_F, SPECIALITE "
                          "FROM FOURNISSEUR "
                          "WHERE TO_CHAR(ID_F) LIKE '%%1%' "
                          "   OR LOWER(NOM_SOCIETE) LIKE LOWER('%%1%') "
                          "   OR LOWER(SPECIALITE) LIKE LOWER('%%1%') "
                          "ORDER BY ID_F").arg(critere);

    model->setQuery(sql);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur dans rechercher():" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom Société");
    model->setHeaderData(2, Qt::Horizontal, "Adresse");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Téléphone");
    model->setHeaderData(5, Qt::Horizontal, "Spécialité");

    return model;
}

QSqlQueryModel* Fournisseur::trier(const QString &champ, const QString &ordre)
{
    QSqlQueryModel *model = new QSqlQueryModel();

    QString column;
    if (champ == "ID" || champ == "id") column = "ID_F";
    else if (champ == "Nom Société" || champ == "nom") column = "NOM_SOCIETE";
    else if (champ == "Spécialité" || champ == "specialite") column = "SPECIALITE";
    else column = "ID_F";

    QString sql = QString("SELECT ID_F, NOM_SOCIETE, ADRESSE_F, MAIL_F, TEL_F, SPECIALITE "
                          "FROM FOURNISSEUR "
                          "ORDER BY %1 %2").arg(column).arg(ordre);

    model->setQuery(sql);

    if (model->lastError().isValid()) {
        qDebug() << "Erreur dans trier():" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Nom Société");
    model->setHeaderData(2, Qt::Horizontal, "Adresse");
    model->setHeaderData(3, Qt::Horizontal, "Email");
    model->setHeaderData(4, Qt::Horizontal, "Téléphone");
    model->setHeaderData(5, Qt::Horizontal, "Spécialité");

    return model;
}

QSqlQueryModel* Fournisseur::statistique()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery("SELECT SPECIALITE, COUNT(*) FROM FOURNISSEUR GROUP BY SPECIALITE");
    return model;
}

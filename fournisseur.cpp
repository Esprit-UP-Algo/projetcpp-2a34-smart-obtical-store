#include "fournisseur.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Fournisseur::Fournisseur() : id(0) {}
Fournisseur::Fournisseur(qint64 id, const QString &nom, const QString &adr,
                         const QString &em, const QString &tel, const QString &spec)
    : id(id), nom_societe(nom), adresse(adr), email(em), telephone(tel), specialite(spec) {}

// Getters
qint64 Fournisseur::getId() const { return id; }
QString Fournisseur::getNomSociete() const { return nom_societe; }
QString Fournisseur::getAdresse() const { return adresse; }
QString Fournisseur::getEmail() const { return email; }
QString Fournisseur::getTelephone() const { return telephone; }
QString Fournisseur::getSpecialite() const { return specialite; }

// Setters
void Fournisseur::setId(qint64 v) { id = v; }
void Fournisseur::setNomSociete(const QString &s) { nom_societe = s; }
void Fournisseur::setAdresse(const QString &s) { adresse = s; }
void Fournisseur::setEmail(const QString &s) { email = s; }
void Fournisseur::setTelephone(const QString &s) { telephone = s; }
void Fournisseur::setSpecialite(const QString &s) { specialite = s; }

bool Fournisseur::ajouter() const
{
    QSqlQuery q;
    q.prepare("INSERT INTO FOURNISSEUR (ID, NOM_SOCIETE, ADRESSE, EMAIL, TELEPHONE, SPECIALITE) "
              "VALUES (:id, :nom, :adresse, :email, :telephone, :specialite)");
    q.bindValue(":id", QVariant::fromValue(id));
    q.bindValue(":nom", nom_societe);
    q.bindValue(":adresse", adresse);
    q.bindValue(":email", email);
    q.bindValue(":telephone", telephone);
    q.bindValue(":specialite", specialite);
    bool ok = q.exec();
    if (!ok) qDebug() << "Fournisseur::ajouter error:" << q.lastError().text();
    return ok;
}

QSqlQueryModel* Fournisseur::afficher() const
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID, NOM_SOCIETE, ADRESSE, EMAIL, TELEPHONE, SPECIALITE FROM FOURNISSEUR");
    query.exec();
    model->setQuery(std::move(query));

    // Définir les headers
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Société"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Spécialité"));

    return model;
}

bool Fournisseur::supprimer(qint64 id) const
{
    QSqlQuery q;
    q.prepare("DELETE FROM FOURNISSEUR WHERE ID = :id");
    q.bindValue(":id", QVariant::fromValue(id));
    bool ok = q.exec();
    if (!ok) qDebug() << "Fournisseur::supprimer error:" << q.lastError().text();
    return ok;
}

bool Fournisseur::modifier() const
{
    QSqlQuery q;
    q.prepare("UPDATE FOURNISSEUR SET NOM_SOCIETE=:nom, ADRESSE=:adresse, EMAIL=:email, TELEPHONE=:telephone, SPECIALITE=:specialite WHERE ID=:id");
    q.bindValue(":id", QVariant::fromValue(id));
    q.bindValue(":nom", nom_societe);
    q.bindValue(":adresse", adresse);
    q.bindValue(":email", email);
    q.bindValue(":telephone", telephone);
    q.bindValue(":specialite", specialite);
    bool ok = q.exec();
    if (!ok) qDebug() << "Fournisseur::modifier error:" << q.lastError().text();
    return ok;
}

QSqlQueryModel* Fournisseur::rechercher(const QString &term) const
{
    QSqlQueryModel *model = new QSqlQueryModel();
    bool isNum = false;
    qint64 val = term.toLongLong(&isNum);

    QSqlQuery q;
    if (isNum) {
        q.prepare("SELECT ID, NOM_SOCIETE, ADRESSE, EMAIL, TELEPHONE, SPECIALITE FROM FOURNISSEUR WHERE ID = :id");
        q.bindValue(":id", QVariant::fromValue(val));
    } else {
        q.prepare("SELECT ID, NOM_SOCIETE, ADRESSE, EMAIL, TELEPHONE, SPECIALITE FROM FOURNISSEUR WHERE LOWER(NOM_SOCIETE) LIKE LOWER(:t) OR LOWER(SPECIALITE) LIKE LOWER(:t) OR LOWER(ADRESSE) LIKE LOWER(:t) OR LOWER(EMAIL) LIKE LOWER(:t)");
        q.bindValue(":t", "%" + term + "%");
    }
    q.exec();
    model->setQuery(std::move(q));

    // Définir les headers
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Société"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Spécialité"));

    return model;
}

QSqlQueryModel* Fournisseur::trier(const QString &field, bool asc) const
{
    QString f;
    if (field == "id") f = "ID";
    else if (field == "nom société") f = "NOM_SOCIETE";
    else if (field == "spécialité") f = "SPECIALITE";
    else f = "ID"; // défaut

    QString order = asc ? "ASC" : "DESC";

    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare(QString("SELECT ID, NOM_SOCIETE, ADRESSE, EMAIL, TELEPHONE, SPECIALITE FROM FOURNISSEUR ORDER BY %1 %2").arg(f, order));
    query.exec();
    model->setQuery(std::move(query));

    // Définir les headers
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Société"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Spécialité"));

    return model;
}

QSqlQueryModel* Fournisseur::statistique() const
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT SPECIALITE, COUNT(ID) AS NB_FOURNISSEURS FROM FOURNISSEUR GROUP BY SPECIALITE");
    query.exec();
    model->setQuery(std::move(query));

    // Définir les headers
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Spécialité"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nombre de Fournisseurs"));

    return model;
}

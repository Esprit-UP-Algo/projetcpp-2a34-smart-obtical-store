#include "rdv.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

RDV::RDV() : id(0), id_client(0), date(QDate::currentDate()), heure(QTime::currentTime()) {}

RDV::RDV(int id, const QString &description, const QDate &date,
         const QTime &heure, int id_client)
    : id(id), id_client(id_client), description(description), date(date), heure(heure)
{
}

bool RDV::ajouter()
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO RDV (ID, DESCRIPTION, DATE_R, HEURE, ID_CLIENT) "
        "VALUES (:id, :desc, TO_DATE(:date, 'YYYY-MM-DD'), "
        "TO_DATE(:heure, 'HH24:MI:SS'), :id_client)"
        );

    query.bindValue(":id", id);
    query.bindValue(":desc", description);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":heure", heure.toString("hh:mm:ss"));
    query.bindValue(":id_client", id_client);

    if (!query.exec()) {
        qDebug() << "Erreur ajout:" << query.lastError().text();
        return false;
    }
    return true;
}

bool RDV::modifier()
{
    QSqlQuery query;
    query.prepare(
        "UPDATE RDV SET DESCRIPTION = :desc, "
        "DATE_R = TO_DATE(:date, 'YYYY-MM-DD'), "
        "HEURE = TO_DATE(:heure, 'HH24:MI:SS'), "
        "ID_CLIENT = :id_client WHERE ID = :id"
        );

    query.bindValue(":id", id);
    query.bindValue(":desc", description);
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":heure", heure.toString("hh:mm:ss"));
    query.bindValue(":id_client", id_client);

    return query.exec();
}

bool RDV::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM RDV WHERE ID = :id");
    query.bindValue(":id", id);
    return query.exec();
}

QSqlQueryModel* RDV::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT ID, DESCRIPTION, "
        "TO_CHAR(DATE_R, 'DD/MM/YYYY') AS DATE_R, "
        "TO_CHAR(HEURE, 'HH24:MI') AS HEURE, "
        "ID_CLIENT FROM RDV ORDER BY DATE_R, HEURE"
        );

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Description");
    model->setHeaderData(2, Qt::Horizontal, "Date");
    model->setHeaderData(3, Qt::Horizontal, "Heure");
    model->setHeaderData(4, Qt::Horizontal, "ID Client");

    return model;
}

QSqlQueryModel* RDV::rechercher(const QString &critere, const QString &valeur)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = "SELECT ID, DESCRIPTION, TO_CHAR(DATE_R, 'DD/MM/YYYY'), "
                  "TO_CHAR(HEURE, 'HH24:MI'), ID_CLIENT FROM RDV WHERE ";
    QString val = valeur;

    if (critere == "ID") sql += "ID = :val";
    else if (critere == "Date") { sql += "TO_CHAR(DATE_R, 'DD/MM/YYYY') LIKE :val"; val = "%" + val + "%"; }
    else if (critere == "Heure") { sql += "TO_CHAR(HEURE, 'HH24:MI') LIKE :val"; val = "%" + val + "%"; }
    else return afficher();

    QSqlQuery q;
    q.prepare(sql);
    q.bindValue(":val", val);
    if (q.exec()) model->setQuery(q);
    else model = afficher();

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Description");
    model->setHeaderData(2, Qt::Horizontal, "Date");
    model->setHeaderData(3, Qt::Horizontal, "Heure");
    model->setHeaderData(4, Qt::Horizontal, "ID Client");

    return model;
}

QSqlQueryModel* RDV::trier(const QString &critere, const QString &ordre)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = "SELECT ID, DESCRIPTION, TO_CHAR(DATE_R, 'DD/MM/YYYY'), "
                  "TO_CHAR(HEURE, 'HH24:MI'), ID_CLIENT FROM RDV ORDER BY ";

    if (critere == "ID") sql += "ID";
    else if (critere == "Date") sql += "DATE_R";
    else if (critere == "Heure") sql += "HEURE";
    else sql += "ID";

    sql += " " + ordre;
    model->setQuery(sql);

    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Description");
    model->setHeaderData(2, Qt::Horizontal, "Date");
    model->setHeaderData(3, Qt::Horizontal, "Heure");
    model->setHeaderData(4, Qt::Horizontal, "ID Client");

    return model;
}

QSqlQueryModel* RDV::statistiqueParJour()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT TO_CHAR(DATE_R, 'DD/MM/YYYY') AS JOUR, COUNT(*) AS NB_RDV "
        "FROM RDV GROUP BY DATE_R ORDER BY DATE_R"
        );
    model->setHeaderData(0, Qt::Horizontal, "Jour");
    model->setHeaderData(1, Qt::Horizontal, "Nb RDV");
    return model;
}

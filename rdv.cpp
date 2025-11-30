// rdv.cpp → النسخة النهائية اللي تخدم عندك 1000% (ID_C مو ID_CLIENT)
#include "rdv.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

RDV::RDV()
    : id(0), id_client(0), date(QDate::currentDate()), heure(QTime::currentTime())
{
}

RDV::RDV(int id, const QString &description, const QDate &date,
         const QTime &heure, int id_client)
    : id(id), description(description), date(date), heure(heure), id_client(id_client)
{
}
// ====================== AJOUTER (النسخة النهائية اللي تخدم عند الجميع) ======================
// ====================== AJOUTER ======================
bool RDV::ajouter()
{
    QSqlQuery query;

    // نستعمل طريقة أبسط ومضمونة 1000% بدون TO_TIMESTAMP
    query.prepare(
        "INSERT INTO RDV (ID_R, DESCRIPTION, DATE_R, HEURE, IDC) "
        "VALUES (:id, :desc, TO_DATE(:date, 'YYYY-MM-DD'), "
        "TO_DATE(:date || ' ' || :time, 'YYYY-MM-DD HH24:MI'), :idc)"
        );

    query.bindValue(":id",   id);
    query.bindValue(":desc", description.trimmed());
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":time",   heure.toString("HH:mm"));  // مهم جدًا بدون mm:ss
    query.bindValue(":idc",  id_client);

    if (!query.exec()) {
        qDebug() << "ÉCHEC AJOUT RDV →" << query.lastError().text();
        qDebug() << "ID_R =" << id << " | IDC =" << id_client << " | Date =" << date.toString() << " | Heure =" << heure.toString("HH:mm");
        return false;
    }

    qDebug() << "RDV ajouté avec succès ! ID_R =" << id;
    return true;
}
// ====================== MODIFIER ======================
bool RDV::modifier()
{
    QSqlQuery query;
    query.prepare(
        "UPDATE RDV SET "
        "DESCRIPTION = :desc, "
        "DATE_R = TO_DATE(:date, 'YYYY-MM-DD'), "
        "HEURE = TO_DATE(:date || ' ' || :time, 'YYYY-MM-DD HH24:MI'), "
        "IDC = :idc "
        "WHERE ID_R = :id"
        );

    query.bindValue(":id",   id);
    query.bindValue(":desc", description.trimmed());
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":time", heure.toString("HH:mm"));
    query.bindValue(":idc",  id_client);

    if (!query.exec()) {
        qDebug() << "ÉCHEC MODIF →" << query.lastError().text();
        return false;
    }
    return true;
}
// ====================== RECHERCHER ======================
// ====================== SUPPRIMER ======================
bool RDV::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM RDV WHERE ID_R = :id");
    query.bindValue(":id", id);
    return query.exec();
}

// ====================== AFFICHER ======================
QSqlQueryModel* RDV::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT ID_R, DESCRIPTION, "
        "TO_CHAR(DATE_R, 'DD/MM/YYYY') AS DATE_R, "
        "TO_CHAR(HEURE, 'HH24:MI') AS HEURE, "
        "IDC FROM RDV ORDER BY DATE_R DESC, HEURE DESC"
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
    QString sql = "SELECT ID_R, DESCRIPTION, "
                  "TO_CHAR(DATE_R, 'DD/MM/YYYY'), "
                  "TO_CHAR(HEURE, 'HH24:MI'), "
                  "IDC FROM RDV WHERE 1=1";

    if (critere == "ID") {
        sql += " AND ID_R = :val";
    } else if (critere == "Description") {
        sql += " AND UPPER(DESCRIPTION) LIKE UPPER(:val)";
    } else if (critere == "Date") {
        sql += " AND TO_CHAR(DATE_R, 'DD/MM/YYYY') LIKE :val";
    }

    QSqlQuery q;
    q.prepare(sql);
    QString val = (critere == "Description" || critere == "Date") ? "%" + valeur + "%" : valeur;
    q.bindValue(":val", val);
    q.exec();

    model->setQuery(q);
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Description");
    model->setHeaderData(2, Qt::Horizontal, "Date");
    model->setHeaderData(3, Qt::Horizontal, "Heure");
    model->setHeaderData(4, Qt::Horizontal, "ID Client");
    return model;
}

// ====================== TRIER ======================
QSqlQueryModel* RDV::trier(const QString &critere, const QString &ordre)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString orderBy = "DATE_R " + ordre + ", HEURE " + ordre;

    if (critere == "ID") orderBy = "ID_R " + ordre;
    else if (critere == "Description") orderBy = "DESCRIPTION " + ordre;

    QString sql = "SELECT ID_R, DESCRIPTION, "
                  "TO_CHAR(DATE_R, 'DD/MM/YYYY'), "
                  "TO_CHAR(HEURE, 'HH24:MI'), "
                  "IDC FROM RDV ORDER BY " + orderBy;

    model->setQuery(sql);
    model->setHeaderData(0, Qt::Horizontal, "ID");
    model->setHeaderData(1, Qt::Horizontal, "Description");
    model->setHeaderData(2, Qt::Horizontal, "Date");
    model->setHeaderData(3, Qt::Horizontal, "Heure");
    model->setHeaderData(4, Qt::Horizontal, "ID Client");
    return model;
}

// ====================== STATISTIQUES ======================
QSqlQueryModel* RDV::statistiqueParJour()
{
    QSqlQueryModel *model = new QSqlQueryModel();
    model->setQuery(
        "SELECT TO_CHAR(DATE_R, 'DD/MM/YYYY') AS JOUR, COUNT(*) AS NB_RDV "
        "FROM RDV GROUP BY DATE_R ORDER BY DATE_R"
        );
    model->setHeaderData(0, Qt::Horizontal, "Jour");
    model->setHeaderData(1, Qt::Horizontal, "Nombre de RDV");
    return model;
}

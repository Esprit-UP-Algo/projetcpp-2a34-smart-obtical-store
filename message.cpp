#include "message.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Constructors
Message::Message()
{
    idMessage = 0;
    idExpediteur = 0;
    idDestinataire = 0;
    sujet = "";
    contenu = "";
    dateEnvoi = QDateTime::currentDateTime();
    lu = false;
}

Message::Message(int idExp, int idDest, QString sujet, QString contenu)
{
    this->idMessage = 0;
    this->idExpediteur = idExp;
    this->idDestinataire = idDest;
    this->sujet = sujet;
    this->contenu = contenu;
    this->dateEnvoi = QDateTime::currentDateTime();
    this->lu = false;
}

Message::Message(int id, int idExp, int idDest, QString sujet, QString contenu, QDateTime date, bool lu)
{
    this->idMessage = id;
    this->idExpediteur = idExp;
    this->idDestinataire = idDest;
    this->sujet = sujet;
    this->contenu = contenu;
    this->dateEnvoi = date;
    this->lu = lu;
}

// Envoyer un message
bool Message::envoyer()
{
    qDebug() << "=== Sending Message ===";

    QSqlQuery query;
    query.prepare("INSERT INTO MESSAGES (ID_MESSAGE, ID_EXPEDITEUR, ID_DESTINATAIRE, SUJET, CONTENU, DATE_ENVOI, LU) "
                  "VALUES (SEQ_MESSAGES.NEXTVAL, :idExp, :idDest, :sujet, :contenu, SYSDATE, 0)");

    query.bindValue(":idExp", idExpediteur);
    query.bindValue(":idDest", idDestinataire);
    query.bindValue(":sujet", sujet);
    query.bindValue(":contenu", contenu);

    if (query.exec())
    {
        qDebug() << "✓ Message sent successfully";
        return true;
    }
    else
    {
        qDebug() << "✗ Error sending message:" << query.lastError().text();
        return false;
    }
}

// Afficher messages reçus
QSqlQueryModel* Message::afficherMessagesRecus(int idEmploye)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString queryStr = QString(
                           "SELECT m.ID_MESSAGE, "
                           "e.NOM_EMPLOYE || ' ' || e.PRENOM_EMPLOYE as EXPEDITEUR, "
                           "m.SUJET, "
                           "m.DATE_ENVOI, "
                           "CASE WHEN m.LU = 0 THEN 'Non lu' ELSE 'Lu' END as STATUT "
                           "FROM MESSAGES m "
                           "JOIN EMPLOYÉ e ON m.ID_EXPEDITEUR = e.ID "
                           "WHERE m.ID_DESTINATAIRE = %1 "
                           "ORDER BY m.DATE_ENVOI DESC"
                           ).arg(idEmploye);

    model->setQuery(queryStr);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("De"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Sujet"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}

// Afficher messages envoyés
QSqlQueryModel* Message::afficherMessagesEnvoyes(int idEmploye)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString queryStr = QString(
                           "SELECT m.ID_MESSAGE, "
                           "e.NOM_EMPLOYE || ' ' || e.PRENOM_EMPLOYE as DESTINATAIRE, "
                           "m.SUJET, "
                           "m.DATE_ENVOI "
                           "FROM MESSAGES m "
                           "JOIN EMPLOYÉ e ON m.ID_DESTINATAIRE = e.ID "
                           "WHERE m.ID_EXPEDITEUR = %1 "
                           "ORDER BY m.DATE_ENVOI DESC"
                           ).arg(idEmploye);

    model->setQuery(queryStr);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("À"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Sujet"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

// Afficher messages non lus
QSqlQueryModel* Message::afficherMessagesNonLus(int idEmploye)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString queryStr = QString(
                           "SELECT m.ID_MESSAGE, "
                           "e.NOM_EMPLOYE || ' ' || e.PRENOM_EMPLOYE as EXPEDITEUR, "
                           "m.SUJET, "
                           "m.DATE_ENVOI "
                           "FROM MESSAGES m "
                           "JOIN EMPLOYÉ e ON m.ID_EXPEDITEUR = e.ID "
                           "WHERE m.ID_DESTINATAIRE = %1 AND m.LU = 0 "
                           "ORDER BY m.DATE_ENVOI DESC"
                           ).arg(idEmploye);

    model->setQuery(queryStr);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("De"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Sujet"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

// Marquer comme lu
bool Message::marquerCommeLu(int idMessage)
{
    QSqlQuery query;
    query.prepare("UPDATE MESSAGES SET LU = 1 WHERE ID_MESSAGE = :id");
    query.bindValue(":id", idMessage);

    if (query.exec())
    {
        qDebug() << "✓ Message marked as read";
        return true;
    }
    else
    {
        qDebug() << "✗ Error marking message as read:" << query.lastError().text();
        return false;
    }
}

// Supprimer message
bool Message::supprimerMessage(int idMessage)
{
    QSqlQuery query;
    query.prepare("DELETE FROM MESSAGES WHERE ID_MESSAGE = :id");
    query.bindValue(":id", idMessage);

    if (query.exec())
    {
        qDebug() << "✓ Message deleted";
        return true;
    }
    else
    {
        qDebug() << "✗ Error deleting message:" << query.lastError().text();
        return false;
    }
}

// Get message by ID
Message Message::getMessageById(int idMessage)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM MESSAGES WHERE ID_MESSAGE = :id");
    query.bindValue(":id", idMessage);

    Message msg;
    if (query.exec() && query.next())
    {
        msg.setIdMessage(query.value(0).toInt());
        msg.setIdExpediteur(query.value(1).toInt());
        msg.setIdDestinataire(query.value(2).toInt());
        msg.setSujet(query.value(3).toString());
        msg.setContenu(query.value(4).toString());
        msg.setDateEnvoi(query.value(5).toDateTime());
        msg.setLu(query.value(6).toInt() == 1);
    }

    return msg;
}

// Compter messages non lus
int Message::compterMessagesNonLus(int idEmploye)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM MESSAGES WHERE ID_DESTINATAIRE = :id AND LU = 0");
    query.bindValue(":id", idEmploye);

    if (query.exec() && query.next())
    {
        return query.value(0).toInt();
    }

    return 0;
}

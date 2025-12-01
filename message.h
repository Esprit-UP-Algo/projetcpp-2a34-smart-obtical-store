#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QDateTime>
#include <QSqlQueryModel>

class Message
{
private:
    int idMessage;
    int idExpediteur;
    int idDestinataire;
    QString sujet;
    QString contenu;
    QDateTime dateEnvoi;
    bool lu;

public:
    // Constructors
    Message();
    Message(int idExp, int idDest, QString sujet, QString contenu);
    Message(int id, int idExp, int idDest, QString sujet, QString contenu, QDateTime date, bool lu);

    // Getters
    int getIdMessage() const { return idMessage; }
    int getIdExpediteur() const { return idExpediteur; }
    int getIdDestinataire() const { return idDestinataire; }
    QString getSujet() const { return sujet; }
    QString getContenu() const { return contenu; }
    QDateTime getDateEnvoi() const { return dateEnvoi; }
    bool isLu() const { return lu; }

    // Setters
    void setIdMessage(int id) { idMessage = id; }
    void setIdExpediteur(int id) { idExpediteur = id; }
    void setIdDestinataire(int id) { idDestinataire = id; }
    void setSujet(QString s) { sujet = s; }
    void setContenu(QString c) { contenu = c; }
    void setDateEnvoi(QDateTime date) { dateEnvoi = date; }
    void setLu(bool l) { lu = l; }

    // CRUD Operations
    bool envoyer();
    static QSqlQueryModel* afficherMessagesRecus(int idEmploye);
    static QSqlQueryModel* afficherMessagesEnvoyes(int idEmploye);
    static QSqlQueryModel* afficherMessagesNonLus(int idEmploye);
    static bool marquerCommeLu(int idMessage);
    static bool supprimerMessage(int idMessage);
    static Message getMessageById(int idMessage);
    static int compterMessagesNonLus(int idEmploye);
};

#endif // MESSAGE_H

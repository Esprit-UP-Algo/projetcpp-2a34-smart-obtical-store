#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Fournisseur
{
private:
    int id;
    QString nom_societe, adresse, email, telephone, specialite;

public:
    Fournisseur();
    Fournisseur(int, QString, QString, QString, QString, QString);

    // Getters
    int getId();
    QString getNomSociete();
    QString getAdresse();
    QString getEmail();
    QString getTelephone();
    QString getSpecialite();

    // Setters
    void setId(int);
    void setNomSociete(QString);
    void setAdresse(QString);
    void setEmail(QString);
    void setTelephone(QString);
    void setSpecialite(QString);

    // Méthodes CRUD
    bool ajouter();
    QSqlQueryModel *afficher();
    bool supprimer(int);
    bool modifier();
};

#endif // FOURNISSEUR_H

#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QString>
#include <QSqlQueryModel>

class Fournisseur
{
public:
    Fournisseur();
    Fournisseur(int id, const QString &nom, const QString &adresse = "",
                const QString &email = "", const QString &telephone = "", const QString &specialite = "");

    int getId() const;
    QString getNom() const;
    QString getAdresse() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getSpecialite() const;

    void setId(int i);
    void setNom(const QString &n);
    void setAdresse(const QString &a);
    void setEmail(const QString &e);
    void setTelephone(const QString &t);
    void setSpecialite(const QString &s);

    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    bool annuler(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(const QString &critere);
    QSqlQueryModel* trier(const QString &champ, const QString &ordre);
    QSqlQueryModel* statistique();

private:
    int id;  // Changé de qint64 à int
    QString nom, adresse, email, telephone, specialite;
};

#endif // FOURNISSEUR_H

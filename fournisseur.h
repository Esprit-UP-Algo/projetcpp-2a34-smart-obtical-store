#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QString>
#include <QSqlQueryModel>

class Fournisseur
{
private:
    int id;
    QString nom_societe, adresse, email, telephone, specialite;

public:
    Fournisseur();
    Fournisseur(int, QString, QString, QString, QString, QString);

    // getters / setters
    int getId() const;
    QString getNomSociete() const;
    QString getAdresse() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getSpecialite() const;

    void setId(int);
    void setNomSociete(const QString &);
    void setAdresse(const QString &);
    void setEmail(const QString &);
    void setTelephone(const QString &);
    void setSpecialite(const QString &);

    // CRUD
    bool ajouter() const;
    // afficher avec filtre (WHERE) et ordre (ORDER BY)
    QSqlQueryModel *afficher(const QString &filter = QString(), const QString &orderBy = QString()) const;
    bool supprimer(int id) const;
    bool modifier() const;

    // utilitaires
    static QSqlQueryModel* chercher(const QString &term); // recherche globale
};
#endif // FOURNISSEUR_H

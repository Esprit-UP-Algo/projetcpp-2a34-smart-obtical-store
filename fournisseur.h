#ifndef FOURNISSEUR_H
#define FOURNISSEUR_H

#include <QString>
#include <QSqlQueryModel>

class Fournisseur
{
public:
    Fournisseur();
    Fournisseur(qint64 id, const QString &nom, const QString &adresse,
                const QString &email, const QString &telephone, const QString &specialite);

    qint64 getId() const;
    QString getNomSociete() const;
    QString getAdresse() const;
    QString getEmail() const;
    QString getTelephone() const;
    QString getSpecialite() const;

    void setId(qint64 v);
    void setNomSociete(const QString &s);
    void setAdresse(const QString &s);
    void setEmail(const QString &s);
    void setTelephone(const QString &s);
    void setSpecialite(const QString &s);

    // CRUD
    bool ajouter() const;
    bool modifier() const;
    bool supprimer(qint64 id) const;
    QSqlQueryModel* afficher() const;
    QSqlQueryModel* rechercher(const QString &term) const;
    QSqlQueryModel* trier(const QString &field, bool asc) const;
    QSqlQueryModel* statistique() const;

private:
    qint64 id;
    QString nom_societe;
    QString adresse;
    QString email;
    QString telephone;
    QString specialite;
};

#endif // FOURNISSEUR_H

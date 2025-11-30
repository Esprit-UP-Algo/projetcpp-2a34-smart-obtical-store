#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>
#include <QSqlQueryModel>
#include <QSqlQuery>

class Produit
{
private:
    int reference;
    QString categorie;
    QString marque;
    double prix;
    int quantite;

public:
    Produit();
    Produit(int ref, QString cat, QString mar, double pr, int qte);

    // Getters
    int getReference() const { return reference; }
    QString getCategorie() const { return categorie; }
    QString getMarque() const { return marque; }
    double getPrix() const { return prix; }
    int getQuantite() const { return quantite; }

    // Setters
    void setReference(int ref) { reference = ref; }
    void setCategorie(const QString &cat) { categorie = cat; }
    void setMarque(const QString &mar) { marque = mar; }
    void setPrix(double pr) { prix = pr; }
    void setQuantite(int qte) { quantite = qte; }

    // Méthodes CRUD
    bool ajouter();
    bool modifier();
    static bool supprimer(int ref);
    static QSqlQueryModel* afficher();
    static QSqlQueryModel* rechercher(const QString &critere);
    static QSqlQueryModel* trier(const QString &critere, const QString &ordre);
    static bool exporterPDF(const QString &filePath);
};

#endif // PRODUIT_H

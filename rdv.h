
// rdv.h
#ifndef RDV_H
#define RDV_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QSqlQueryModel>

    class RDV
{
public:
    RDV();
    // CETTE LIGNE DOIT ÊTRE EXACTEMENT COMME ÇA
    RDV(int id, const QString &description, const QDate &date,
        const QTime &heure, int id_client);

    // Getters
    int getId() const { return id; }
    QString getDescription() const { return description; }
    QDate getDate() const { return date; }
    QTime getHeure() const { return heure; }
    int getIdClient() const { return id_client; }

    // Setters
    void setId(int id) { this->id = id; }
    void setDescription(const QString &desc) { description = desc; }
    void setDate(const QDate &d) { date = d; }
    void setHeure(const QTime &h) { heure = h; }
    void setIdClient(int idc) { id_client = idc; }

    // Méthodes CRUD
    bool ajouter();
    bool modifier();
    static bool supprimer(int id);
    static QSqlQueryModel* afficher();
    static QSqlQueryModel* rechercher(const QString &critere, const QString &valeur);
    static QSqlQueryModel* trier(const QString &critere, const QString &ordre);
    static QSqlQueryModel* statistiqueParJour();

private:
    int id;
    QString description;
    QDate date;
    QTime heure;
    int id_client;
};

#endif // RDV_H

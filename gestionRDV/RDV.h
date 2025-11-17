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
    RDV(int id, const QString &description, const QDate &date,
        const QTime &heure, int id_client);

    int getId() const { return id; }
    QString getDescription() const { return description; }
    QDate getDate() const { return date; }
    QTime getHeure() const { return heure; }
    int getIdClient() const { return id_client; }

    void setId(int id) { this->id = id; }
    void setDescription(const QString &desc) { description = desc; }
    void setDate(const QDate &d) { date = d; }
    void setHeure(const QTime &h) { heure = h; }
    void setIdClient(int idc) { id_client = idc; }

    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(const QString &critere, const QString &valeur);
    QSqlQueryModel* trier(const QString &critere, const QString &ordre);
    QSqlQueryModel* statistiqueParJour();

private:
    int id;
    int id_client;
    QString description;
    QDate date;
    QTime heure;
};

#endif // RDV_H

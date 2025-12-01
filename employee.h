#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlQueryModel>
#include <QMap>

class Employee
{
private:
    int id;
    QString nomEmploye;
    QString prenomEmploye;
    int age;
    int telephone;
    QString poste;
    QString mail;
    QString motDePasse;

public:
    // Constructors
    Employee();
    Employee(int id, QString nomEmploye, QString prenomEmploye, int age,
             int telephone, QString poste, QString mail, QString motDePasse);
    Employee(QString nomEmploye, QString prenomEmploye, int age,
             int telephone, QString poste, QString mail, QString motDePasse);

    // Getters
    int getId() const { return id; }
    QString getNomEmploye() const { return nomEmploye; }
    QString getPrenomEmploye() const { return prenomEmploye; }
    int getAge() const { return age; }
    int getTelephone() const { return telephone; }
    QString getPoste() const { return poste; }
    QString getMail() const { return mail; }
    QString getMotDePasse() const { return motDePasse; }

    // Setters
    void setId(int id) { this->id = id; }
    void setNomEmploye(QString nomEmploye) { this->nomEmploye = nomEmploye; }
    void setPrenomEmploye(QString prenomEmploye) { this->prenomEmploye = prenomEmploye; }
    void setAge(int age) { this->age = age; }
    void setTelephone(int telephone) { this->telephone = telephone; }
    void setPoste(QString poste) { this->poste = poste; }
    void setMail(QString mail) { this->mail = mail; }
    void setMotDePasse(QString motDePasse) { this->motDePasse = motDePasse; }

    // CRUD Operations
    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    QSqlQueryModel* afficher();

    // Additional useful methods
    Employee getEmployeeById(int id);
    QSqlQueryModel* rechercher(QString critere);
    QSqlQueryModel* trierPar(QString colonne, QString ordre = "ASC");
    QMap<QString, int> getStatistiquesParPoste();
};

#endif // EMPLOYEE_H

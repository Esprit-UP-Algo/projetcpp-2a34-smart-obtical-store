#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidget>
#include <QTableWidgetItem>
#include "connection.h"

class Client
{
private:
    int id;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;
    int age;
    QString adresse;
    int pointsFidelite;

public:
    // Constructeur
    Client();

    // ==================== GETTERS ====================
    int getId() const { return id; }
    QString getNom() const { return nom; }
    QString getPrenom() const { return prenom; }
    QString getEmail() const { return email; }
    QString getTelephone() const { return telephone; }
    int getAge() const { return age; }
    QString getAdresse() const { return adresse; }
    int getPointsFidelite() const { return pointsFidelite; }

    // ==================== SETTERS ====================
    void setId(int v) { id = v; }
    void setNom(const QString &v) { nom = v; }
    void setPrenom(const QString &v) { prenom = v; }
    void setEmail(const QString &v) { email = v; }
    void setTelephone(const QString &v) { telephone = v; }
    void setAge(int v) { age = v; }
    void setAdresse(const QString &v) { adresse = v; }
    void setPointsFidelite(int v) { pointsFidelite = v; }

    // ==================== VALIDATIONS ====================
    bool isValidId() const { return id >= 100000 && id <= 999999; }
    bool isValidAge() const { return age >= 0 && age <= 999; }
    bool isValidPoints() const { return pointsFidelite >= 0 && pointsFidelite <= 9999; }
    bool isValidTelephone() const { return telephone.length() == 8; }
    bool isValidEmail() const { return email.contains("@") && email.contains("."); }

    // ==================== CRUD ====================
    bool ajouter(QString *errMsg = nullptr);
    bool modifier(QString *errMsg = nullptr);
    static bool supprimer(int id, QString *errMsg = nullptr);
    static bool afficher(QTableWidget *table,
                         const QString &orderBy,
                         bool asc,
                         const QString &search,
                         const QString &criteria,
                         int &out_countRows,
                         int &out_c12_18,
                         int &out_c19_30,
                         int &out_c31_50,
                         int &out_c50_plus,
                         QString *errMsg = nullptr);
};

#endif // CLIENT_H

#include "employee.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// Default Constructor
Employee::Employee()
{
    id = 0;
    nomEmploye = "";
    prenomEmploye = "";
    age = 0;
    telephone = 0;
    poste = "";
    mail = "";
    motDePasse = "";
}

// Constructor with all parameters including ID
Employee::Employee(int id, QString nomEmploye, QString prenomEmploye, int age,
                   int telephone, QString poste, QString mail, QString motDePasse)
{
    this->id = id;
    this->nomEmploye = nomEmploye;
    this->prenomEmploye = prenomEmploye;
    this->age = age;
    this->telephone = telephone;
    this->poste = poste;
    this->mail = mail;
    this->motDePasse = motDePasse;
}

// Constructor without ID (for adding new employees)
Employee::Employee(QString nomEmploye, QString prenomEmploye, int age,
                   int telephone, QString poste, QString mail, QString motDePasse)
{
    this->id = 0;
    this->nomEmploye = nomEmploye;
    this->prenomEmploye = prenomEmploye;
    this->age = age;
    this->telephone = telephone;
    this->poste = poste;
    this->mail = mail;
    this->motDePasse = motDePasse;
}

// CREATE - Ajouter un employé
bool Employee::ajouter()
{
    qDebug() << "=== Starting Employee Add ===";

    QSqlQuery query;

    // Since ID is nullable, we can either insert NULL or generate an ID
    // Let's try inserting NULL first to see if Oracle handles it
    query.prepare("INSERT INTO EMPLOYÉ (ID, NOM_EMPLOYE, PRENOM_EMPLOYE, AGE, TELEPHONE, POSTE, MAIL, MOT_DE_PASSE) "
                  "VALUES (NULL, :nomEmploye, :prenomEmploye, :age, :telephone, :poste, :mail, :motDePasse)");

    query.bindValue(":nomEmploye", nomEmploye);
    query.bindValue(":prenomEmploye", prenomEmploye);
    query.bindValue(":age", age);
    query.bindValue(":telephone", telephone);
    query.bindValue(":poste", poste);
    query.bindValue(":mail", mail);
    query.bindValue(":motDePasse", motDePasse);

    qDebug() << "Attempting insert with NULL ID...";

    if (query.exec())
    {
        qDebug() << "✓✓✓ Employé ajouté avec succès! ✓✓✓";
        qDebug() << "Nom:" << nomEmploye << prenomEmploye;
        return true;
    }
    else
    {
        qDebug() << "Insert with NULL failed, trying with generated ID...";

        // If NULL doesn't work, generate an ID manually
        QSqlQuery maxIdQuery;
        int nextId = 1;

        if (maxIdQuery.exec("SELECT NVL(MAX(ID), 0) + 1 FROM EMPLOYÉ"))
        {
            if (maxIdQuery.next())
            {
                nextId = maxIdQuery.value(0).toInt();
            }
        }

        qDebug() << "Generated ID:" << nextId;

        query.prepare("INSERT INTO EMPLOYÉ (ID, NOM_EMPLOYE, PRENOM_EMPLOYE, AGE, TELEPHONE, POSTE, MAIL, MOT_DE_PASSE) "
                      "VALUES (:id, :nomEmploye, :prenomEmploye, :age, :telephone, :poste, :mail, :motDePasse)");

        query.bindValue(":id", nextId);
        query.bindValue(":nomEmploye", nomEmploye);
        query.bindValue(":prenomEmploye", prenomEmploye);
        query.bindValue(":age", age);
        query.bindValue(":telephone", telephone);
        query.bindValue(":poste", poste);
        query.bindValue(":mail", mail);
        query.bindValue(":motDePasse", motDePasse);

        if (query.exec())
        {
            qDebug() << "✓✓✓ Employé ajouté avec succès! ✓✓✓";
            qDebug() << "ID:" << nextId << "Nom:" << nomEmploye << prenomEmploye;
            return true;
        }
        else
        {
            qDebug() << "✗✗✗ Erreur lors de l'ajout ✗✗✗";
            qDebug() << "Error:" << query.lastError().text();
            return false;
        }
    }
}

// READ - Afficher tous les employés
QSqlQueryModel* Employee::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM EMPLOYÉ ORDER BY ID");

    // Set header labels in French
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Mot de Passe"));

    return model;
}

// UPDATE - Modifier un employé
bool Employee::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYÉ SET NOM_EMPLOYE=:nomEmploye, PRENOM_EMPLOYE=:prenomEmploye, AGE=:age, "
                  "TELEPHONE=:telephone, POSTE=:poste, MAIL=:mail, MOT_DE_PASSE=:motDePasse "
                  "WHERE ID=:id");

    query.bindValue(":id", id);
    query.bindValue(":nomEmploye", nomEmploye);
    query.bindValue(":prenomEmploye", prenomEmploye);
    query.bindValue(":age", age);
    query.bindValue(":telephone", telephone);
    query.bindValue(":poste", poste);
    query.bindValue(":mail", mail);
    query.bindValue(":motDePasse", motDePasse);

    if (query.exec())
    {
        qDebug() << "Employé modifié avec succès!";
        return true;
    }
    else
    {
        qDebug() << "Erreur lors de la modification:" << query.lastError().text();
        return false;
    }
}

// DELETE - Supprimer un employé
bool Employee::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYÉ WHERE ID=:id");
    query.bindValue(":id", id);

    if (query.exec())
    {
        qDebug() << "Employé supprimé avec succès!";
        return true;
    }
    else
    {
        qDebug() << "Erreur lors de la suppression:" << query.lastError().text();
        return false;
    }
}

// Get employee by ID
Employee Employee::getEmployeeById(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYÉ WHERE ID=:id");
    query.bindValue(":id", id);

    Employee emp;
    if (query.exec() && query.next())
    {
        emp.setId(query.value(0).toInt());
        emp.setNomEmploye(query.value(1).toString());
        emp.setPrenomEmploye(query.value(2).toString());
        emp.setAge(query.value(3).toInt());
        emp.setTelephone(query.value(4).toInt());
        emp.setPoste(query.value(5).toString());
        emp.setMail(query.value(6).toString());
        emp.setMotDePasse(query.value(7).toString());
    }
    else
    {
        qDebug() << "Erreur lors de la récupération:" << query.lastError().text();
    }

    return emp;
}

// Search employees
// Search employees
QSqlQueryModel* Employee::rechercher(QString critere)
{
    qDebug() << "=== Search Function Called ===";
    qDebug() << "Search criteria:" << critere;

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // Check if the search term is a number
    bool isNumber = false;
    int searchId = critere.toInt(&isNumber);

    if (isNumber)
    {
        // Search by ID only
        qDebug() << "Searching by ID:" << searchId;
        query.prepare("SELECT * FROM EMPLOYÉ WHERE ID = :id");
        query.bindValue(":id", searchId);
    }
    else
    {
        // Search by Name, Poste, Phone (text search)
        qDebug() << "Searching by text:" << critere;
        query.prepare("SELECT * FROM EMPLOYÉ WHERE "
                      "LOWER(NOM_EMPLOYE) LIKE :critere OR "
                      "LOWER(PRENOM_EMPLOYE) LIKE :critere OR "
                      "LOWER(POSTE) LIKE :critere OR "
                      "CAST(TELEPHONE AS VARCHAR(20)) LIKE :critere");

        QString searchPattern = "%" + critere.toLower() + "%";
        query.bindValue(":critere", searchPattern);
    }

    qDebug() << "Executing query...";

    if (!query.exec())
    {
        qDebug() << "✗ Search query FAILED!";
        qDebug() << "Error:" << query.lastError().text();
    }
    else
    {
        qDebug() << "✓ Query executed successfully";
    }

    model->setQuery(query);
    qDebug() << "Results found:" << model->rowCount();

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Mot de Passe"));

    return model;
}

// Sort employees
QSqlQueryModel* Employee::trierPar(QString colonne, QString ordre)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT * FROM EMPLOYÉ ORDER BY " + colonne + " " + ordre;
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Age"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Poste"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Mail"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Mot de Passe"));

    return model;
}

// Get statistics by poste
QMap<QString, int> Employee::getStatistiquesParPoste()
{
    qDebug() << "=== Getting Statistics by Poste ===";

    QMap<QString, int> stats;
    QSqlQuery query;

    query.prepare("SELECT POSTE, COUNT(*) as COUNT FROM EMPLOYÉ GROUP BY POSTE ORDER BY POSTE");

    if (query.exec())
    {
        while (query.next())
        {
            QString poste = query.value(0).toString();
            int count = query.value(1).toInt();
            stats[poste] = count;
            qDebug() << "Poste:" << poste << "Count:" << count;
        }
    }
    else
    {
        qDebug() << "Error getting statistics:" << query.lastError().text();
    }

    return stats;
}

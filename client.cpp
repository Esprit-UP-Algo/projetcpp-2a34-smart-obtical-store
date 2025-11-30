#include "client.h"
#include <QDebug>

// ==================== CONSTRUCTEUR ====================
Client::Client()
    : id(0), age(0), pointsFidelite(0)
{
}

// ==================== CRUD ====================

// ✅ AJOUTER (CREATE)
bool Client::ajouter(QString *errMsg)
{
    Connection &conn = Connection::getInstance();
    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return false;
    }

    QSqlQuery query(conn.getDatabase());
    query.prepare("INSERT INTO CLIENT (IDC, NOMC, PRENOMC, MAILC, TELEPHONEC, AGEC, ADRESSEC, POINTS_CLIENTS) "
                  "VALUES (:id, :nom, :prenom, :mail, :tel, :age, :adr, :pts)");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":mail", email);
    query.bindValue(":tel", telephone);
    query.bindValue(":age", age);
    query.bindValue(":adr", adresse);
    query.bindValue(":pts", pointsFidelite);

    if (!query.exec()) {
        if (errMsg) *errMsg = "Erreur SQL : " + query.lastError().text();
        return false;
    }
    return true;
}

// ✏️ MODIFIER (UPDATE)
bool Client::modifier(QString *errMsg)
{
    Connection &conn = Connection::getInstance();
    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return false;
    }

    QSqlQuery query(conn.getDatabase());
    query.prepare("UPDATE CLIENT SET NOMC=:nom, PRENOMC=:prenom, MAILC=:mail, "
                  "TELEPHONEC=:tel, AGEC=:age, ADRESSEC=:adr, POINTS_CLIENTS=:pts "
                  "WHERE IDC=:id");

    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":mail", email);
    query.bindValue(":tel", telephone);
    query.bindValue(":age", age);
    query.bindValue(":adr", adresse);
    query.bindValue(":pts", pointsFidelite);

    if (!query.exec()) {
        if (errMsg) *errMsg = "Erreur SQL : " + query.lastError().text();
        return false;
    }
    return true;
}

// 🗑️ SUPPRIMER (DELETE)
bool Client::supprimer(int id, QString *errMsg)
{
    Connection &conn = Connection::getInstance();
    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return false;
    }

    QSqlQuery query(conn.getDatabase());
    query.prepare("DELETE FROM CLIENT WHERE IDC=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        if (errMsg) *errMsg = "Erreur SQL : " + query.lastError().text();
        return false;
    }
    return true;
}

// 📋 AFFICHER (READ) avec recherche, tri et statistiques
bool Client::afficher(QTableWidget *table,
                      const QString &orderBy,
                      bool asc,
                      const QString &search,
                      const QString &criteria,
                      int &out_countRows,
                      int &out_c12_18,
                      int &out_c19_30,
                      int &out_c31_50,
                      int &out_c50_plus,
                      QString *errMsg)
{
    if(!table) {
        if(errMsg) *errMsg = "Table widget null";
        return false;
    }

    Connection &conn = Connection::getInstance();
    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return false;
    }

    QString queryStr = "SELECT IDC, NOMC, PRENOMC, MAILC, TELEPHONEC, AGEC, ADRESSEC, POINTS_CLIENTS FROM CLIENT";

    if(!search.isEmpty()){
        if(criteria.compare("ID", Qt::CaseInsensitive) == 0)
            queryStr += " WHERE TO_CHAR(IDC) LIKE :s";
        else if(criteria.compare("telephone", Qt::CaseInsensitive) == 0)
            queryStr += " WHERE TELEPHONEC LIKE :s";
        else if(criteria.compare("mail", Qt::CaseInsensitive) == 0)
            queryStr += " WHERE MAILC LIKE :s";
    }

    if(!orderBy.isEmpty()){
        queryStr += " ORDER BY " + orderBy + (asc ? " ASC" : " DESC");
    }

    QSqlQuery q(conn.getDatabase());
    if(!q.prepare(queryStr)) {
        if(errMsg) *errMsg = "Erreur prepare SQL: " + q.lastError().text();
        return false;
    }
    if(!search.isEmpty()) q.bindValue(":s", "%" + search + "%");

    if(!q.exec()){
        if(errMsg) *errMsg = "Erreur exec SQL: " + q.lastError().text();
        return false;
    }

    table->setRowCount(0);
    table->setSortingEnabled(false);

    int row = 0;
    out_countRows = 0;
    out_c12_18 = out_c19_30 = out_c31_50 = out_c50_plus = 0;

    while(q.next()){
        table->insertRow(row);

        for(int col=0; col<8; ++col){
            QTableWidgetItem *it = new QTableWidgetItem(q.value(col).toString());

            if(col==0||col==4||col==5||col==7)
                it->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            else
                it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            table->setItem(row, col, it);
        }

        int ageVal = q.value(5).toInt();
        if(ageVal >= 12 && ageVal <= 18) out_c12_18++;
        else if(ageVal >= 19 && ageVal <= 30) out_c19_30++;
        else if(ageVal >= 31 && ageVal <= 50) out_c31_50++;
        else if(ageVal > 50) out_c50_plus++;

        row++;
    }

    out_countRows = row;
    table->setSortingEnabled(true);
    return true;
}

// ==================== FONCTIONS UTILITAIRES ====================

// 🔢 COMPTER TOTAL
int Client::compterTotal(QString *errMsg)
{
    Connection &conn = Connection::getInstance();
    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return -1;
    }

    QSqlQuery query(conn.getDatabase());
    query.prepare("SELECT COUNT(*) FROM CLIENT");

    if (!query.exec()) {
        if (errMsg) *errMsg = "Erreur SQL : " + query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// ✔️ VÉRIFIER SI ID EXISTE
bool Client::idExiste(int id, QString *errMsg)
{
    Connection &conn = Connection::getInstance();
    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return false;
    }

    QSqlQuery query(conn.getDatabase());
    query.prepare("SELECT COUNT(*) FROM CLIENT WHERE IDC = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        if (errMsg) *errMsg = "Erreur SQL : " + query.lastError().text();
        return false;
    }

    if (query.next()) {
        return query.value(0).toInt() > 0;
    }
    return false;
}

// 🔍 RÉCUPÉRER CLIENT PAR ID
Client Client::getById(int id, QString *errMsg)
{
    Client c;
    Connection &conn = Connection::getInstance();

    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return c;
    }

    QSqlQuery query(conn.getDatabase());
    query.prepare("SELECT IDC, NOMC, PRENOMC, MAILC, TELEPHONEC, AGEC, ADRESSEC, POINTS_CLIENTS "
                  "FROM CLIENT WHERE IDC = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        if (errMsg) *errMsg = "Erreur SQL : " + query.lastError().text();
        return c;
    }

    if (query.next()) {
        c.setId(query.value(0).toInt());
        c.setNom(query.value(1).toString());
        c.setPrenom(query.value(2).toString());
        c.setEmail(query.value(3).toString());
        c.setTelephone(query.value(4).toString());
        c.setAge(query.value(5).toInt());
        c.setAdresse(query.value(6).toString());
        c.setPointsFidelite(query.value(7).toInt());
    }

    return c;
}

// 📊 STATISTIQUES COMPLÈTES
Client::ClientStats Client::getStatistiques(QString *errMsg)
{
    ClientStats stats = {0, 0, 0, 0, 0, 0.0, 0};

    Connection &conn = Connection::getInstance();
    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return stats;
    }

    QSqlQuery query(conn.getDatabase());
    query.prepare("SELECT AGEC, POINTS_CLIENTS FROM CLIENT");

    if (!query.exec()) {
        if (errMsg) *errMsg = "Erreur SQL : " + query.lastError().text();
        return stats;
    }

    int totalAge = 0;
    int totalPoints = 0;

    while (query.next()) {
        int age = query.value(0).toInt();
        int points = query.value(1).toInt();

        stats.total++;
        totalAge += age;
        totalPoints += points;

        if (age >= 12 && age <= 18) stats.c12_18++;
        else if (age >= 19 && age <= 30) stats.c19_30++;
        else if (age >= 31 && age <= 50) stats.c31_50++;
        else if (age > 50) stats.c50_plus++;
    }

    if (stats.total > 0) {
        stats.moyenneAge = static_cast<double>(totalAge) / stats.total;
        stats.moyennePoints = totalPoints / stats.total;
    }

    return stats;
}

// 📄 EXPORT CSV
bool Client::exporterCSV(const QString &filepath, QString *errMsg)
{
    Connection &conn = Connection::getInstance();
    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return false;
    }

    QSqlQuery query(conn.getDatabase());
    query.prepare("SELECT IDC, NOMC, PRENOMC, MAILC, TELEPHONEC, AGEC, ADRESSEC, POINTS_CLIENTS "
                  "FROM CLIENT ORDER BY NOMC ASC");

    if (!query.exec()) {
        if (errMsg) *errMsg = "Erreur SQL : " + query.lastError().text();
        return false;
    }

    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (errMsg) *errMsg = "Impossible d'ouvrir le fichier : " + filepath;
        return false;
    }

    QTextStream out(&file);
    out << "ID,Nom,Prenom,Email,Telephone,Age,Adresse,Points\n";

    while (query.next()) {
        out << query.value(0).toString() << ","
            << "\"" << query.value(1).toString() << "\","
            << "\"" << query.value(2).toString() << "\","
            << query.value(3).toString() << ","
            << query.value(4).toString() << ","
            << query.value(5).toString() << ","
            << "\"" << query.value(6).toString() << "\","
            << query.value(7).toString() << "\n";
    }

    file.close();
    return true;
}

// 🔎 RECHERCHE AVANCÉE
QList<Client> Client::rechercherAvance(const QString &terme, QString *errMsg)
{
    QList<Client> liste;
    Connection &conn = Connection::getInstance();

    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return liste;
    }

    QSqlQuery query(conn.getDatabase());
    query.prepare("SELECT IDC, NOMC, PRENOMC, MAILC, TELEPHONEC, AGEC, ADRESSEC, POINTS_CLIENTS "
                  "FROM CLIENT WHERE "
                  "TO_CHAR(IDC) LIKE :t OR "
                  "UPPER(NOMC) LIKE UPPER(:t) OR "
                  "UPPER(PRENOMC) LIKE UPPER(:t) OR "
                  "UPPER(MAILC) LIKE UPPER(:t) OR "
                  "TELEPHONEC LIKE :t");

    query.bindValue(":t", "%" + terme + "%");

    if (!query.exec()) {
        if (errMsg) *errMsg = "Erreur SQL : " + query.lastError().text();
        return liste;
    }

    while (query.next()) {
        Client c;
        c.setId(query.value(0).toInt());
        c.setNom(query.value(1).toString());
        c.setPrenom(query.value(2).toString());
        c.setEmail(query.value(3).toString());
        c.setTelephone(query.value(4).toString());
        c.setAge(query.value(5).toInt());
        c.setAdresse(query.value(6).toString());
        c.setPointsFidelite(query.value(7).toInt());
        liste.append(c);
    }

    return liste;
}

// ➕ AJOUTER POINTS FIDÉLITÉ
bool Client::ajouterPoints(int id, int points, QString *errMsg)
{
    Connection &conn = Connection::getInstance();
    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return false;
    }

    QSqlQuery query(conn.getDatabase());
    query.prepare("UPDATE CLIENT SET POINTS_CLIENTS = POINTS_CLIENTS + :pts WHERE IDC = :id");
    query.bindValue(":pts", points);
    query.bindValue(":id", id);

    if (!query.exec()) {
        if (errMsg) *errMsg = "Erreur SQL : " + query.lastError().text();
        return false;
    }

    return true;
}

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
    // 1️⃣ Récupérer la connexion Singleton
    Connection &conn = Connection::getInstance();
    if (!conn.createConnection()) {
        if (errMsg) *errMsg = "Erreur connexion BD";
        return false;
    }

    // 2️⃣ Préparer la requête (Protection SQL Injection)
    QSqlQuery query(conn.getDatabase());
    query.prepare("INSERT INTO CLIENT (IDC, NOMC, PRENOMC, MAILC, TELEPHONEC, AGEC, ADRESSEC, POINTS_CLIENTS) "
                  "VALUES (:id, :nom, :prenom, :mail, :tel, :age, :adr, :pts)");

    // 3️⃣ Lier les valeurs (bindValue = Protection)
    query.bindValue(":id", id);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":mail", email);
    query.bindValue(":tel", telephone);
    query.bindValue(":age", age);
    query.bindValue(":adr", adresse);
    query.bindValue(":pts", pointsFidelite);

    // 4️⃣ Exécuter et vérifier
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

    // 1️⃣ Construire la requête dynamique
    QString queryStr = "SELECT IDC, NOMC, PRENOMC, MAILC, TELEPHONEC, AGEC, ADRESSEC, POINTS_CLIENTS FROM CLIENT";

    // 2️⃣ Ajouter le filtre de recherche
    if(!search.isEmpty()){
        if(criteria.compare("ID", Qt::CaseInsensitive) == 0)
            queryStr += " WHERE TO_CHAR(IDC) LIKE :s";
        else if(criteria.compare("telephone", Qt::CaseInsensitive) == 0)
            queryStr += " WHERE TELEPHONEC LIKE :s";
        else if(criteria.compare("mail", Qt::CaseInsensitive) == 0)
            queryStr += " WHERE MAILC LIKE :s";
    }

    // 3️⃣ Ajouter le tri
    if(!orderBy.isEmpty()){
        queryStr += " ORDER BY " + orderBy + (asc ? " ASC" : " DESC");
    }

    // 4️⃣ Préparer et exécuter
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

    // 5️⃣ Vider le tableau
    table->setRowCount(0);
    table->setSortingEnabled(false);

    // 6️⃣ Initialiser les compteurs
    int row = 0;
    out_countRows = 0;
    out_c12_18 = out_c19_30 = out_c31_50 = out_c50_plus = 0;

    // 7️⃣ Remplir le tableau ligne par ligne
    while(q.next()){
        table->insertRow(row);

        // Remplir les 8 colonnes
        for(int col=0; col<8; ++col){
            QTableWidgetItem *it = new QTableWidgetItem(q.value(col).toString());

            // Alignement: nombres à droite, texte à gauche
            if(col==0||col==4||col==5||col==7)  // ID, Tel, Age, Points
                it->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
            else
                it->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            table->setItem(row, col, it);
        }

        // 8️⃣ Calculer les statistiques par âge
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

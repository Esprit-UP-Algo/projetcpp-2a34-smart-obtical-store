#include "produit.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QPdfWriter>
#include <QPainter>
#include <QFont>
#include <QPageSize>
#include <QDateTime>  // ← AJOUTEZ CETTE LIGNE

Produit::Produit()
    : reference(0), prix(0.0), quantite(0)
{
}

Produit::Produit(int ref, QString cat, QString mar, double pr, int qte)
    : reference(ref), categorie(cat), marque(mar), prix(pr), quantite(qte)
{
}

// ✅ AJOUTER
bool Produit::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO PRODUIT (REFERENCE, CATEGORIE, MARQUE, PRIX, QUANTITE) "
                  "VALUES (:ref, :cat, :marq, :pr, :qt)");
    query.bindValue(":ref", reference);
    query.bindValue(":cat", categorie);
    query.bindValue(":marq", marque);
    query.bindValue(":pr", prix);
    query.bindValue(":qt", quantite);

    return query.exec();
}

// ✅ MODIFIER
bool Produit::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE PRODUIT SET CATEGORIE=:cat, MARQUE=:marq, PRIX=:pr, QUANTITE=:qt "
                  "WHERE REFERENCE=:ref");
    query.bindValue(":ref", reference);
    query.bindValue(":cat", categorie);
    query.bindValue(":marq", marque);
    query.bindValue(":pr", prix);
    query.bindValue(":qt", quantite);

    return query.exec();
}

// ✅ SUPPRIMER (statique)
bool Produit::supprimer(int ref)
{
    QSqlQuery query;
    query.prepare("DELETE FROM PRODUIT WHERE REFERENCE=:ref");
    query.bindValue(":ref", ref);

    return query.exec();
}

// ✅ AFFICHER
QSqlQueryModel* Produit::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT REFERENCE, CATEGORIE, MARQUE, PRIX, QUANTITE FROM PRODUIT";
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, "Référence");
    model->setHeaderData(1, Qt::Horizontal, "Catégorie");
    model->setHeaderData(2, Qt::Horizontal, "Marque");
    model->setHeaderData(3, Qt::Horizontal, "Prix");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");

    return model;
}

// ✅ RECHERCHE (statique)
QSqlQueryModel* Produit::rechercher(const QString &critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    QString queryStr = QString("SELECT REFERENCE, CATEGORIE, MARQUE, PRIX, QUANTITE FROM PRODUIT "
                               "WHERE REFERENCE LIKE '%%1%' OR CATEGORIE LIKE '%%1%' OR MARQUE LIKE '%%1%'")
                           .arg(critere);

    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, "Référence");
    model->setHeaderData(1, Qt::Horizontal, "Catégorie");
    model->setHeaderData(2, Qt::Horizontal, "Marque");
    model->setHeaderData(3, Qt::Horizontal, "Prix");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");

    return model;
}

// ✅ TRIER (statique)
QSqlQueryModel* Produit::trier(const QString &critere, const QString &ordre)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = QString("SELECT REFERENCE, CATEGORIE, MARQUE, PRIX, QUANTITE FROM PRODUIT "
                               "ORDER BY %1 %2").arg(critere).arg(ordre);
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, "Référence");
    model->setHeaderData(1, Qt::Horizontal, "Catégorie");
    model->setHeaderData(2, Qt::Horizontal, "Marque");
    model->setHeaderData(3, Qt::Horizontal, "Prix");
    model->setHeaderData(4, Qt::Horizontal, "Quantité");

    return model;
}

// ✅ EXPORTER PDF (statique)
bool Produit::exporterPDF(const QString &filePath)
{
    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setTitle("Liste des Produits");

    QPainter painter(&pdf);
    if (!painter.isActive()) {
        return false;
    }

    // Configuration des polices
    QFont titleFont("Arial", 16, QFont::Bold);
    QFont headerFont("Arial", 10, QFont::Bold);
    QFont dataFont("Arial", 9);

    // En-tête du document
    painter.setFont(titleFont);
    painter.drawText(100, 100, "LISTE DES PRODUITS");

    painter.setFont(dataFont);
    painter.drawText(100, 120, QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm"));

    painter.setFont(headerFont);
    int y = 170;

    // En-têtes du tableau
    painter.drawText(100, y, "Référence");
    painter.drawText(180, y, "Catégorie");
    painter.drawText(300, y, "Marque");
    painter.drawText(420, y, "Prix");
    painter.drawText(500, y, "Quantité");

    y += 20;
    painter.drawLine(100, y, 580, y);
    y += 30;

    // Données
    painter.setFont(dataFont);

    QSqlQuery query("SELECT REFERENCE, CATEGORIE, MARQUE, PRIX, QUANTITE FROM PRODUIT ORDER BY REFERENCE");

    while (query.next()) {
        QString ref = query.value(0).toString();
        QString cat = query.value(1).toString();
        QString marque = query.value(2).toString();
        double prix = query.value(3).toDouble();
        int quantite = query.value(4).toInt();

        // Dessiner chaque colonne séparément
        painter.drawText(100, y, ref);
        painter.drawText(180, y, cat);
        painter.drawText(300, y, marque);
        painter.drawText(420, y, QString("%1 DT").arg(prix, 0, 'f', 2));
        painter.drawText(500, y, QString::number(quantite));

        y += 25;

        // Nouvelle page si nécessaire
        if (y > 700) {
            pdf.newPage();
            y = 100;

            // Redessiner les en-têtes
            painter.setFont(headerFont);
            painter.drawText(100, y, "Référence");
            painter.drawText(180, y, "Catégorie");
            painter.drawText(300, y, "Marque");
            painter.drawText(420, y, "Prix");
            painter.drawText(500, y, "Quantité");

            y += 20;
            painter.drawLine(100, y, 580, y);
            y += 30;
            painter.setFont(dataFont);
        }
    }

    // Pied de page
    painter.setFont(QFont("Arial", 8));
    painter.drawText(100, 750, QString("Total: %1 produits").arg(query.size()));

    painter.end();
    return true;
}

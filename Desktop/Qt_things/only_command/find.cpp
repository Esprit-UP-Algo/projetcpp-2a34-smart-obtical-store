#include "find.h"
#include <QSqlQuery>
#include <QTableWidget>
#include <QTableWidgetItem>

QSqlQueryModel* FindCommande::rechercher(const QString &type, const QString &valeur)
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;

    QString sql = "SELECT ref_c, mode_payaement, TO_CHAR(date_commande,'DD/MM/YYYY'), "
                  "montant_totale, etat, montant_a_payer FROM commande WHERE 1=1";

    if (type == "Ref") {
        sql += " AND ref_c LIKE :val";
    } else if (type == "Date") {
        sql += " AND TO_CHAR(date_commande,'DD/MM/YYYY') LIKE :val";
    } else if (type == "Total") {
        sql += " AND montant_totale LIKE :val";
    }

    query.prepare(sql);
    query.bindValue(":val", "%" + valeur + "%");

    if (query.exec()) {
        model->setQuery(std::move(query));
        model->setHeaderData(0, Qt::Horizontal, "Référence");
        model->setHeaderData(1, Qt::Horizontal, "Mode Paiement");
        model->setHeaderData(2, Qt::Horizontal, "Date");
        model->setHeaderData(3, Qt::Horizontal, "Total");
        model->setHeaderData(4, Qt::Horizontal, "État");
        model->setHeaderData(5, Qt::Horizontal, "À Payer");
    }

    return model;
}

void FindCommande::chargerTableau(QTableWidget *table)
{
    table->setRowCount(0);
    QSqlQuery query("SELECT ref_c, mode_payaement, TO_CHAR(date_commande,'DD/MM/YYYY'), "
                    "montant_totale, etat, montant_a_payer FROM commande ORDER BY ref_c");

    int row = 0;
    while (query.next()) {
        table->insertRow(row);
        for (int col = 0; col < 6; ++col) {
            table->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}

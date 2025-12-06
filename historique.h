#ifndef HISTORIQUE_H
#define HISTORIQUE_H

#include <QString>
#include <QSqlQueryModel>

class Historique
{
public:
    Historique();

    // Log pour fournisseur
    bool logActionFournisseur(qint64 idFournisseur, const QString &action,
                              const QString &utilisateur, const QString &details = "");

    // Log pour produit (lié au fournisseur)
    bool logActionProduit(qint64 refProduit, qint64 idFournisseur, const QString &action,
                          const QString &utilisateur, const QString &details = "");

    QSqlQueryModel* afficherAll() const;

    QSqlQueryModel* filtrer(QString terme, QString action, QString from, QString to) const;

    QString exportToPdf(QSqlQueryModel *model, const QString &title, const QString &filenamePrefix = "historique");
};

#endif // HISTORIQUE_H


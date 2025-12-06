#include "historique.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QPdfWriter>
#include <QPainter>
#include <QDateTime>
#include <QDebug>
#include <QPageSize>
#include <QPageLayout>

Historique::Historique() {}

bool Historique::logActionFournisseur(qint64 idFournisseur, const QString &action,
                                      const QString &utilisateur, const QString &details)
{
    QSqlQuery q;
    q.prepare(R"(INSERT INTO HISTORIQUE_FOURNISSEUR
                 (ID_FOURNISSEUR, ACTION, DATE_ACTION, UTILISATEUR, DETAILS)
                 VALUES (:id, :action, SYSDATE, :user, :details))");

    q.bindValue(":id", idFournisseur);
    q.bindValue(":action", action);
    q.bindValue(":user", utilisateur);
    q.bindValue(":details", details.isEmpty() ? QVariant(QVariant::String) : details);

    bool ok = q.exec();
    if (!ok) {
        qDebug() << "ERREUR LOG HISTORIQUE:" << q.lastError().text();
    }
    return ok;
}
bool Historique::logActionProduit(qint64 refProduit, qint64 idFournisseur, const QString &action,
                                  const QString &utilisateur, const QString &details)
{
    QSqlQuery q;
    q.prepare("INSERT INTO HISTORIQUE_FOURNISSEUR (ID_FOURNISSEUR, REFERENCE_PRODUIT, ACTION, DATE_ACTION, UTILISATEUR, DETAILS) "
              "VALUES (:idf, :ref, :act, SYSDATE, :user, :det)");
    q.bindValue(":idf", idFournisseur);
    q.bindValue(":ref", refProduit);
    q.bindValue(":act", action);
    q.bindValue(":user", utilisateur);
    q.bindValue(":det", details);
    return q.exec();
}

QSqlQueryModel* Historique::afficherAll() const
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare(R"(
        SELECT
            h.ID_HIST,
            h.ID_FOURNISSEUR,
            f.NOM_SOCIETE,
            f.ADRESSE,
            f.EMAIL,
            f.TELEPHONE,
            f.SPECIALITE,
            h.REFERENCE_PRODUIT,
            p.CATEGORIE || ' ' || p.MARQUE AS PRODUIT,
            h.ACTION,
            TO_CHAR(h.DATE_ACTION, 'DD/MM/YYYY HH24:MI:SS') AS DATE_ACTION,
            h.UTILISATEUR,
            h.DETAILS
        FROM HISTORIQUE_FOURNISSEUR h
        LEFT JOIN FOURNISSEUR f ON h.ID_FOURNISSEUR = f.ID
        LEFT JOIN PRODUIT p ON h.REFERENCE_PRODUIT = p.REFERENCE
        ORDER BY h.DATE_ACTION DESC
    )");
    query.exec();
    model->setQuery(std::move(query));

    // Headers complets avec coordonnées fournisseurs
    model->setHeaderData(0, Qt::Horizontal, "ID Hist");
    model->setHeaderData(1, Qt::Horizontal, "ID Fournisseur");
    model->setHeaderData(2, Qt::Horizontal, "Nom Société");
    model->setHeaderData(3, Qt::Horizontal, "Adresse");
    model->setHeaderData(4, Qt::Horizontal, "Email");
    model->setHeaderData(5, Qt::Horizontal, "Téléphone");
    model->setHeaderData(6, Qt::Horizontal, "Spécialité");
    model->setHeaderData(7, Qt::Horizontal, "Ref Produit");
    model->setHeaderData(8, Qt::Horizontal, "Produit");
    model->setHeaderData(9, Qt::Horizontal, "Action");
    model->setHeaderData(10, Qt::Horizontal, "Date Action");
    model->setHeaderData(11, Qt::Horizontal, "Utilisateur");
    model->setHeaderData(12, Qt::Horizontal, "Détails");
    return model;
}

QSqlQueryModel* Historique::filtrer(QString terme, QString action, QString from, QString to) const
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = R"(
        SELECT
            h.ID_HIST,
            h.ID_FOURNISSEUR,
            f.NOM_SOCIETE,
            f.ADRESSE,
            f.EMAIL,
            f.TELEPHONE,
            f.SPECIALITE,
            h.REFERENCE_PRODUIT,
            p.CATEGORIE || ' ' || p.MARQUE AS PRODUIT,
            h.ACTION,
            TO_CHAR(h.DATE_ACTION, 'DD/MM/YYYY HH24:MI:SS') AS DATE_ACTION,
            h.UTILISATEUR,
            h.DETAILS
        FROM HISTORIQUE_FOURNISSEUR h
        LEFT JOIN FOURNISSEUR f ON h.ID_FOURNISSEUR = f.ID
        LEFT JOIN PRODUIT p ON h.REFERENCE_PRODUIT = p.REFERENCE
        WHERE 1=1
    )";

    if (!terme.isEmpty()) {
        sql += " AND (f.NOM_SOCIETE LIKE :terme OR h.DETAILS LIKE :terme OR h.ACTION LIKE :terme)";
    }
    if (!action.isEmpty() && action != "Tous") {
        sql += " AND h.ACTION = :action";
    }
    if (from != "2000-01-01") {
        sql += " AND h.DATE_ACTION >= TO_DATE(:from, 'YYYY-MM-DD')";
    }
    if (to != QDate::currentDate().toString("yyyy-MM-dd")) {
        sql += " AND h.DATE_ACTION < TO_DATE(:to, 'YYYY-MM-DD') + 1";
    }

    sql += " ORDER BY h.DATE_ACTION DESC";

    QSqlQuery q;
    q.prepare(sql);

    if (!terme.isEmpty()) {
        q.bindValue(":terme", "%" + terme + "%");
    }
    if (!action.isEmpty() && action != "Tous") {
        q.bindValue(":action", action);
    }
    if (from != "2000-01-01") {
        q.bindValue(":from", from);
    }
    if (to != QDate::currentDate().toString("yyyy-MM-dd")) {
        q.bindValue(":to", to);
    }

    q.exec();
    model->setQuery(std::move(q));

    // Même headers que afficherAll()
    model->setHeaderData(0, Qt::Horizontal, "ID Hist");
    model->setHeaderData(1, Qt::Horizontal, "ID Fournisseur");
    model->setHeaderData(2, Qt::Horizontal, "Nom Société");
    model->setHeaderData(9, Qt::Horizontal, "Action");
    model->setHeaderData(10, Qt::Horizontal, "Date Action");
    model->setHeaderData(11, Qt::Horizontal, "Utilisateur");
    model->setHeaderData(12, Qt::Horizontal, "Détails");

    return model;
}

// Export PDF amélioré et bien organisé
QString Historique::exportToPdf(QSqlQueryModel *model, const QString &title, const QString &filenamePrefix)
{
    if (!model || model->rowCount() == 0) return "";

    QString fileName = QString("%1_%2.pdf")
                           .arg(filenamePrefix)
                           .arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    // Marges
    QPageLayout layout = writer.pageLayout();
    layout.setMargins(QMarginsF(20, 20, 20, 20)); // Marges réduites pour plus d'espace
    writer.setPageLayout(layout);

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);

    // === EN-TÊTE ===
    // Titre principal
    QFont titleFont = painter.font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(0, 100, writer.width(), 100, Qt::AlignCenter, title);

    // Sous-titre avec date
    QFont subtitleFont = painter.font();
    subtitleFont.setPointSize(10);
    subtitleFont.setBold(false);
    painter.setFont(subtitleFont);
    painter.drawText(0, 180, writer.width(), 50, Qt::AlignCenter,
                     "Généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm"));

    // Ligne séparatrice
    painter.drawLine(50, 250, writer.width() - 50, 250);

    // === TABLEAU ===
    int startY = 300;
    int rowHeight = 40;
    int headerHeight = 50;

    // Calcul des largeurs de colonnes proportionnelles
    QVector<int> colWidths;
    int totalWidth = writer.width() - 100; // Largeur disponible
    int totalColumns = model->columnCount();

    // Répartition proportionnelle (ajuster selon besoins)
    for (int i = 0; i < totalColumns; ++i) {
        if (i == 2 || i == 3) { // Nom Société et Adresse plus larges
            colWidths << totalWidth * 0.15;
        } else if (i == 12) { // Détails plus large
            colWidths << totalWidth * 0.12;
        } else {
            colWidths << totalWidth * 0.08;
        }
    }

    // === EN-TÊTES DE COLONNES ===
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(QColor(200, 200, 200))); // Gris clair pour l'en-tête

    int xPos = 50;
    for (int col = 0; col < model->columnCount(); ++col) {
        QString header = model->headerData(col, Qt::Horizontal).toString();

        // Dessiner le rectangle de l'en-tête
        painter.drawRect(xPos, startY, colWidths[col], headerHeight);

        // Dessiner le texte de l'en-tête
        QFont headerFont = painter.font();
        headerFont.setPointSize(8);
        headerFont.setBold(true);
        painter.setFont(headerFont);
        painter.drawText(xPos + 5, startY + 5, colWidths[col] - 10, headerHeight - 10,
                         Qt::AlignLeft | Qt::TextWordWrap, header);

        xPos += colWidths[col];
    }

    // === DONNÉES ===
    int currentY = startY + headerHeight;
    QFont dataFont = painter.font();
    dataFont.setPointSize(7);
    dataFont.setBold(false);
    painter.setFont(dataFont);

    for (int row = 0; row < model->rowCount(); ++row) {
        // Nouvelle page si nécessaire
        if (currentY > writer.height() - 100) {
            writer.newPage();
            currentY = 100;

            // Redessiner les en-têtes sur la nouvelle page
            xPos = 50;
            painter.setBrush(QBrush(QColor(200, 200, 200)));
            for (int col = 0; col < model->columnCount(); ++col) {
                QString header = model->headerData(col, Qt::Horizontal).toString();
                painter.drawRect(xPos, currentY, colWidths[col], headerHeight);

                QFont headerFont = painter.font();
                headerFont.setPointSize(8);
                headerFont.setBold(true);
                painter.setFont(headerFont);
                painter.drawText(xPos + 5, currentY + 5, colWidths[col] - 10, headerHeight - 10,
                                 Qt::AlignLeft | Qt::TextWordWrap, header);
                xPos += colWidths[col];
            }
            currentY += headerHeight;
        }

        // Alternance de couleurs pour les lignes
        if (row % 2 == 0) {
            painter.setBrush(QBrush(QColor(240, 240, 240))); // Gris très clair
        } else {
            painter.setBrush(QBrush(Qt::white));
        }

        // Dessiner les données de la ligne
        xPos = 50;
        for (int col = 0; col < model->columnCount(); ++col) {
            QString text = model->data(model->index(row, col)).toString();

            // Dessiner le rectangle de la cellule
            painter.drawRect(xPos, currentY, colWidths[col], rowHeight);

            // Dessiner le texte
            painter.drawText(xPos + 5, currentY + 5, colWidths[col] - 10, rowHeight - 10,
                             Qt::AlignLeft | Qt::TextWordWrap, text);

            xPos += colWidths[col];
        }
        currentY += rowHeight;
    }

    // === PIED DE PAGE ===
    painter.setFont(subtitleFont);
    painter.drawText(0, writer.height() - 50, writer.width(), 30, Qt::AlignCenter,
                     QString("Page 1 - %1 enregistrement(s)").arg(model->rowCount()));

    painter.end();
    return fileName;
}

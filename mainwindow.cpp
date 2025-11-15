// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRegularExpression>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QSqlQueryModel>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPageSize>
#include <QPainter>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Assurer que radiobutton decroissant est décoché au départ
    ui->radiobtnDecroissantFournisseur->setChecked(false);

    // Charger la table principale au démarrage
    refreshMainTable();

    // Optionnel : connecter les clics de la table pour remplir les champs (sélection)
    connect(ui->tableWidgetFournisseur, &QTableWidget::cellClicked, this, [this](int row, int){
        // Remplir les champs avec la ligne sélectionnée si elle existe
        QTableWidget *t = ui->tableWidgetFournisseur;
        if (row < 0 || row >= t->rowCount()) return;
        auto itemId = t->item(row, 0);
        auto itemNom = t->item(row, 1);
        auto itemAdr = t->item(row, 2);
        auto itemEmail = t->item(row, 3);
        auto itemTel = t->item(row, 4);
        auto itemSpec = t->item(row, 5);

        if (itemId) ui->afficheIDFournisseur->setText(itemId->text());
        if (itemNom) ui->affichenomsocieteFournisseur->setText(itemNom->text());
        if (itemAdr) ui->afficheAdresseFournisseur->setText(itemAdr->text());
        if (itemEmail) ui->afficheEmailFournisseur->setText(itemEmail->text());
        if (itemTel) ui->afficheTelephoneFournisseur->setText(itemTel->text());

        // mettre le combo spécialité si correspond
        if (itemSpec) {
            int idx = ui->combospecialiteFournisseur->findText(itemSpec->text());
            if (idx >= 0) ui->combospecialiteFournisseur->setCurrentIndex(idx);
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ========== Helpers ==========

void MainWindow::populateTableFromModel(QSqlQueryModel *model, QTableWidget *table)
{
    if (!model || !table) return;

    int rows = model->rowCount();
    int cols = model->columnCount();

    table->clearContents();
    table->setRowCount(rows);
    table->setColumnCount(cols);

    // Header labels
    QStringList headers;
    for (int c = 0; c < cols; ++c)
        headers << model->headerData(c, Qt::Horizontal).toString();
    table->setHorizontalHeaderLabels(headers);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            QString text = model->data(model->index(r, c)).toString();
            QTableWidgetItem *it = new QTableWidgetItem(text);
            table->setItem(r, c, it);
        }
    }
}

// Validate inputs according to rules from your table
// ID: exactly 12 digits
// Nom société: letters only (with accents), max 20
// Adresse: letters+digits max 50
// Email: contains @, <= 30 chars
// Téléphone: exactly 8 digits
// Spécialité: not empty
bool MainWindow::validateInputs(long long &outId, QString &outNom, QString &outAdresse,
                                QString &outEmail, QString &outTelephone, QString &outSpecialite,
                                QString &errMsg)
{
    errMsg.clear();

    QString idStr = ui->afficheIDFournisseur->text().trimmed();
    QRegularExpression rxId("^[0-9]{12}$");
    if (!rxId.match(idStr).hasMatch()) {
        errMsg = "ID doit contenir exactement 12 chiffres.";
        return false;
    }
    bool okConv = false;
    outId = idStr.toLongLong(&okConv);
    if (!okConv) {
        errMsg = "ID invalide.";
        return false;
    }

    outNom = ui->affichenomsocieteFournisseur->text().trimmed();
    QRegularExpression rxNom("^[A-Za-zÀ-ÿ\\s]{1,20}$");
    if (!rxNom.match(outNom).hasMatch()) {
        errMsg = "Nom société invalide (lettres uniquement, max 20).";
        return false;
    }

    outAdresse = ui->afficheAdresseFournisseur->text().trimmed();
    QRegularExpression rxAdr("^[A-Za-z0-9À-ÿ\\s]{1,50}$");
    if (!rxAdr.match(outAdresse).hasMatch()) {
        errMsg = "Adresse invalide (lettres/chiffres, max 50).";
        return false;
    }

    outEmail = ui->afficheEmailFournisseur->text().trimmed();
    if (outEmail.isEmpty() || outEmail.length() > 30 || !outEmail.contains('@')) {
        errMsg = "Email invalide (doit contenir '@' et <= 30 caractères).";
        return false;
    }

    outTelephone = ui->afficheTelephoneFournisseur->text().trimmed();
    QRegularExpression rxTel("^[0-9]{8}$");
    if (!rxTel.match(outTelephone).hasMatch()) {
        errMsg = "Téléphone invalide (8 chiffres).";
        return false;
    }

    outSpecialite = ui->combospecialiteFournisseur->currentText().trimmed();
    if (outSpecialite.isEmpty()) {
        errMsg = "Spécialité requise.";
        return false;
    }

    // All OK
    return true;
}

void MainWindow::refreshMainTable()
{
    QSqlQueryModel *model = F.afficher();
    populateTableFromModel(model, ui->tableWidgetFournisseur);
    delete model;
}

// ========== Slots (CRUD) ==========

void MainWindow::on_btnAjoutFournisseur_clicked()
{
    long long id;
    QString nom, adresse, email, tel, spec, err;
    if (!validateInputs(id, nom, adresse, email, tel, spec, err)) {
        QMessageBox::warning(this, "Validation", err);
        return;
    }

    // ATTENTION: si ta classe Fournisseur attend un int, considère modifier Fournisseur pour utiliser qint64.
    // Ici on cast en int — si ton ID dépasse INT_MAX, ce cast échouera.
    Fournisseur f(static_cast<int>(id), nom, adresse, email, tel, spec);
    if (f.ajouter()) {
        QMessageBox::information(this, "Ajout", "Fournisseur ajouté avec succès.");
        refreshMainTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout (voir console).");
    }
}

void MainWindow::on_btnModifierFournisseur_clicked()
{
    long long id;
    QString nom, adresse, email, tel, spec, err;
    if (!validateInputs(id, nom, adresse, email, tel, spec, err)) {
        QMessageBox::warning(this, "Validation", err);
        return;
    }

    Fournisseur f(static_cast<int>(id), nom, adresse, email, tel, spec);
    if (f.modifier()) {
        QMessageBox::information(this, "Modification", "Fournisseur modifié.");
        refreshMainTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.");
    }
}

void MainWindow::on_btnSupprimerFournisseur_clicked()
{
    QString idStr = ui->afficheIDFournisseur->text().trimmed();
    QRegularExpression rxId("^[0-9]{12}$");
    if (!rxId.match(idStr).hasMatch()) {
        QMessageBox::warning(this, "Validation", "Entrer un ID valide (12 chiffres) pour supprimer.");
        return;
    }
    long long id = idStr.toLongLong();
    if (F.supprimer(static_cast<int>(id))) {
        QMessageBox::information(this, "Suppression", "Fournisseur supprimé.");
        refreshMainTable();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression.");
    }
}

void MainWindow::on_btnAnnulerFournisseur_clicked()
{
    ui->afficheIDFournisseur->clear();
    ui->affichenomsocieteFournisseur->clear();
    ui->afficheAdresseFournisseur->clear();
    ui->afficheEmailFournisseur->clear();
    ui->afficheTelephoneFournisseur->clear();
    ui->combospecialiteFournisseur->setCurrentIndex(0);
    ui->afficheRechercheFournisseur->clear();
}

// ========== Recherche & Tri ==========

void MainWindow::on_btnRechercheFournisseur_clicked()
{
    QString term = ui->afficheRechercheFournisseur->text().trimmed();
    if (term.isEmpty()) {
        refreshMainTable();
        return;
    }
    QSqlQueryModel *model = F.rechercher(term);
    populateTableFromModel(model, ui->tableWidgetFournisseur);
    delete model;
}

void MainWindow::on_btnTriFournisseur_clicked()
{
    QString selected = ui->comboTriFournisseur->currentText().toLower();
    QString critere;
    if (selected.contains("id")) critere = "ID";
    else if (selected.contains("nom")) critere = "NOM_SOCIETE";
    else if (selected.contains("spécial") || selected.contains("special")) critere = "SPECIALITE";
    else critere = "ID";

    bool asc = !ui->radiobtnDecroissantFournisseur->isChecked();
    QString ordre = asc ? "ASC" : "DESC";

    QSqlQueryModel *model = F.trier(critere, ordre);
    populateTableFromModel(model, ui->tableWidgetFournisseur);
    delete model;
}

// ========== Export PDF (Qt6 compatible) ==========

void MainWindow::on_btnExporterPDFFournisseur_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Exporter en PDF"), QString(),
                                                    tr("PDF Files (*.pdf)"));
    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);
    QFont titleFont = painter.font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    painter.setFont(titleFont);

    int x = 40;
    int y = 60;
    painter.drawText(x, y, "Liste des Fournisseurs");
    y += 40;

    // Header font
    QFont headerFont = painter.font();
    headerFont.setPointSize(10);
    headerFont.setBold(true);
    painter.setFont(headerFont);

    QTableWidget *table = ui->tableWidgetFournisseur;
    int cols = table->columnCount();
    int rows = table->rowCount();

    // compute column widths (simple equal widths)
    int pageWidth = writer.pageLayout().fullRectPixels(writer.resolution()).width();
    int margin = 40;
    int usable = pageWidth - margin * 2;
    int colWidth = qMax(80, usable / qMax(1, cols));

    // draw headers
    for (int c = 0; c < cols; ++c) {
        QString h = table->horizontalHeaderItem(c) ? table->horizontalHeaderItem(c)->text() : QString("Col%1").arg(c);
        painter.drawText(x + c * colWidth, y, h);
    }
    y += 25;

    // rows
    QFont cellFont = painter.font(); cellFont.setBold(false); cellFont.setPointSize(9);
    painter.setFont(cellFont);

    for (int r = 0; r < rows; ++r) {
        // page break if needed
        if (y + 30 > writer.height()) {
            writer.newPage();
            y = 60;
        }
        for (int c = 0; c < cols; ++c) {
            QString text;
            auto it = table->item(r, c);
            if (it) text = it->text();
            painter.drawText(x + c * colWidth, y, text);
        }
        y += 20;
    }

    painter.end();
    QMessageBox::information(this, "Export PDF", "Export terminé : " + fileName);
}

// ========== Statistique (par spécialité) ==========

void MainWindow::on_btnStatistiqueFournisseur_clicked()
{
    QSqlQueryModel *model = F.statistique();
    // populate the stat table
    populateTableFromModel(model, ui->tableWidgetStatFournisseur);

    // Optionally add percentage column computed locally:
    // compute total
    int total = 0;
    for (int r = 0; r < model->rowCount(); ++r) {
        bool ok = false;
        int count = model->data(model->index(r, 1)).toInt(&ok);
        if (ok) total += count;
    }

    // If we want to show % column in the stat table
    if (total > 0) {
        // Add a column "Pourcentage"
        int oldCols = ui->tableWidgetStatFournisseur->columnCount();
        ui->tableWidgetStatFournisseur->setColumnCount(oldCols + 1);
        QStringList headers;
        for (int c = 0; c < oldCols; ++c)
            headers << ui->tableWidgetStatFournisseur->horizontalHeaderItem(c)->text();
        headers << "Pourcentage";
        ui->tableWidgetStatFournisseur->setHorizontalHeaderLabels(headers);

        for (int r = 0; r < ui->tableWidgetStatFournisseur->rowCount(); ++r) {
            int cnt = ui->tableWidgetStatFournisseur->item(r, 1)->text().toInt();
            double pct = (double)cnt * 100.0 / (double)total;
            QString pctStr = QString::number(pct, 'f', 2) + " %";
            ui->tableWidgetStatFournisseur->setItem(r, 2, new QTableWidgetItem(pctStr));
        }
    }

    delete model;
}

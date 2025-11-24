#include "mainwindow.h"
#include "ui_mainwindow.h"
/*#include <QPrinter>
#include <QPrintDialog>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QFileDialog>*/
#include <QMessageBox>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include "delete.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // === Database connection ===
    if (!conn.createconnect()) {
        QMessageBox::critical(this, "Erreur", "Impossible de se connecter à la base de données.");
        close();
        return;
    }

    // Fill combo boxes
    ui->mod_->addItems({"CASH", "CARD", "CHECK", "ONLINE"});
    ui->etat_payment->addItems({"payé", "non payé"});
    ui->recherche_type->addItems({"Ref", "Date", "Total"});

    // Load all data
    chargerTableau();

    // Connect buttons
    connect(ui->add,            &QPushButton::clicked, this, &MainWindow::on_add_clicked);
    connect(ui->update,         &QPushButton::clicked, this, &MainWindow::on_update_clicked);
    connect(ui->cancel,         &QPushButton::clicked, this, &MainWindow::on_cancel_clicked);
    connect(ui->recherchebutton,&QPushButton::clicked, this, &MainWindow::on_recherchebutton_clicked);
    connect(ui->pushButton_10,  &QPushButton::clicked, this, &MainWindow::on_pushButton_10_clicked);
    connect(ui->tab_show_command,&QTableWidget::cellClicked, this, &MainWindow::on_tab_show_command_cellClicked);
    connect(ui->supprimer, &QPushButton::clicked, this, &MainWindow::on_supprimer_clicked);
    //connect(ui->imprimer, &QPushButton::clicked, this, &MainWindow::on_imprimer_clicked);
}

MainWindow::~MainWindow() { delete ui; }

// =================================================================
// ADD
// =================================================================
void MainWindow::on_add_clicked()
{
    Commande c;
    bool ok;

    c.ref_c = ui->ref->text().toInt(&ok);
    if (!ok || c.ref_c <= 0) { QMessageBox::warning(this,"Erreur","Référence invalide !"); return; }

    c.id_c = ui->id_client->text().toInt(&ok);
    if (!ok || c.id_c <= 0) { QMessageBox::warning(this,"Erreur","ID Client invalide !"); return; }

    c.mode_paiement = ui->mod_->currentText();
    c.date_commande = QDate::fromString(ui->date->text(), "dd/MM/yyyy");
    if (!c.date_commande.isValid()) { QMessageBox::warning(this,"Erreur","Date invalide (jj/mm/aaaa) !"); return; }

    c.montant_total = ui->m_tot->text().toDouble(&ok);
    if (!ok || c.montant_total < 0) { QMessageBox::warning(this,"Erreur","Montant total invalide !"); return; }

    c.etat = ui->etat_payment->currentText();
    c.montant_a_payer = ui->montant_a_payyer->text().toDouble(&ok);
    if (!ok) c.montant_a_payer = 0.0;

    if (AddCommande::ajouter(c)) {
        chargerTableau();
        viderFormulaire();
    }
}

// =================================================================
// UPDATE
// =================================================================
void MainWindow::on_update_clicked()
{
    Commande c;
    bool ok;

    c.ref_c = ui->ref->text().toInt(&ok);
    if (!ok || c.ref_c <= 0) {
        QMessageBox::warning(this,"Erreur","Sélectionnez une commande à modifier !");
        return;
    }

    c.id_c = ui->id_client->text().toInt(&ok);
    if (!ok || c.id_c <= 0) { QMessageBox::warning(this,"Erreur","ID Client invalide !"); return; }

    c.mode_paiement = ui->mod_->currentText();
    c.date_commande = QDate::fromString(ui->date->text(), "dd/MM/yyyy");
    if (!c.date_commande.isValid()) { QMessageBox::warning(this,"Erreur","Date invalide !"); return; }

    c.montant_total = ui->m_tot->text().toDouble(&ok);
    if (!ok) { QMessageBox::warning(this,"Erreur","Montant total invalide !"); return; }

    c.etat = ui->etat_payment->currentText();
    c.montant_a_payer = ui->montant_a_payyer->text().toDouble(&ok);
    if (!ok) c.montant_a_payer = 0.0;

    if (UpdateCommande::modifier(c)) {
        chargerTableau();
    }
}

// =================================================================
// CANCEL
// =================================================================
void MainWindow::on_cancel_clicked() { viderFormulaire(); }

// =================================================================
// SEARCH
// =================================================================
void MainWindow::on_recherchebutton_clicked()
{
    QString type = ui->recherche_type->currentText();
    QString valeur = ui->recherche->text().trimmed();

    QSqlQueryModel *model = FindCommande::rechercher(type, valeur);

    ui->tab_show_command->setRowCount(0);  // Clear table

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tab_show_command->insertRow(row);
        for (int col = 0; col < model->columnCount(); ++col) {
            ui->tab_show_command->setItem(row, col,
                                          new QTableWidgetItem(model->data(model->index(row, col)).toString()));
        }
    }
    delete model;
    ui->tab_show_command->resizeColumnsToContents();
}

// =================================================================
// REFRESH / SORT (temporary)
// =================================================================
void MainWindow::on_pushButton_10_clicked()
{
    chargerTableau();
}

// =================================================================
// CLICK ON ROW → FILL FORM
// =================================================================
void MainWindow::on_tab_show_command_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    ui->ref->setText(ui->tab_show_command->item(row, 0)->text());
    ui->mod_->setCurrentText(ui->tab_show_command->item(row, 1)->text());
    ui->date->setText(ui->tab_show_command->item(row, 2)->text());
    ui->m_tot->setText(ui->tab_show_command->item(row, 3)->text());
    ui->etat_payment->setCurrentText(ui->tab_show_command->item(row, 4)->text());
    ui->montant_a_payyer->setText(ui->tab_show_command->item(row, 5)->text());

    // Get hidden ID_c
    int ref = ui->ref->text().toInt();
    QSqlQuery q;
    q.prepare("SELECT ID_c FROM commande WHERE ref_c = :ref");
    q.bindValue(":ref", ref);
    if (q.exec() && q.next()) {
        ui->id_client->setText(q.value(0).toString());
    }
}

// =================================================================
// LOAD ALL DATA
// =================================================================
void MainWindow::chargerTableau()
{
    FindCommande::chargerTableau(ui->tab_show_command);
    ui->tab_show_command->resizeColumnsToContents();
}
// =================================================================
// DELETE COMMAND
// =================================================================
void MainWindow::on_supprimer_clicked()
{
    // Make sure a row is selected
    int row = ui->tab_show_command->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Aucune sélection", "Veuillez sélectionner une commande à supprimer !");
        return;
    }

    // Get reference from the selected row
    QTableWidgetItem *item = ui->tab_show_command->item(row, 0);
    if (!item) return;

    int ref = item->text().toInt();

    // Confirmation dialog
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmer la suppression",
                                  QString("Voulez-vous vraiment supprimer la commande n° %1 ?").arg(ref),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (DeleteCommande::supprimer(ref)) {
            chargerTableau();        // Refresh table
            viderFormulaire();       // Clear form
            QMessageBox::information(this, "Succès", "Commande supprimée avec succès !");
        }
    }
}

// =================================================================
// PRINT / EXPORT PDF
// =================================================================
/*void MainWindow::on_imprimer_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setOutputFormat(QPrinter::NativeFormat);

    // Option 1: Direct print dialog
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        painter.setRenderHint(QPainter::Antialiasing);

        // Print the table
        ui->tab_show_command->render(&painter);
        painter.end();

        QMessageBox::information(this, "Impression", "Impression lancée avec succès !");
        return;
    }

    // Option 2: If user cancels print → ask for PDF export
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Exporter en PDF ?",
                                  "Voulez-vous exporter la liste des commandes en PDF ?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        "Exporter en PDF", "Commandes_Liste.pdf", "Fichiers PDF (*.pdf)");

        if (!fileName.isEmpty()) {
            if (!fileName.endsWith(".pdf", Qt::CaseInsensitive))
                fileName += ".pdf";

            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setOutputFileName(fileName);

            QPainter painter(&printer);
            ui->tab_show_command->render(&painter);
            painter.end();

            QMessageBox::information(this, "PDF Exporté",
                                     QString("Liste exportée avec succès :\n%1").arg(fileName));
        }
    }
}
*/
// =================================================================
// CLEAR FORM
// =================================================================
void MainWindow::viderFormulaire()
{
    ui->ref->clear();
    ui->id_client->clear();
    ui->date->clear();
    ui->m_tot->clear();
    ui->montant_a_payyer->clear();
    ui->recherche->clear();
    ui->mod_->setCurrentIndex(0);
    ui->etat_payment->setCurrentIndex(0);
}

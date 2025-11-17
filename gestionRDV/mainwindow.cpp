#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QPdfWriter>
#include <QPainter>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->menubtnFournisseur, &QPushButton::clicked, this, &MainWindow::on_menubtnFournisseur_clicked);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit_2->setTime(QTime::currentTime());
    refreshRdvTable();

    // CLIC SUR LIGNE
    connect(ui->tableWidget_8, &QTableWidget::cellClicked, this, [this](int row, int) {
        if (row < 0) return;
        auto t = ui->tableWidget_8;

        ui->lineEdit_43->setText(t->item(row, 0)->text());
        ui->lineEdit_48->setText(t->item(row, 1)->text());

        QString dateStr = t->item(row, 2)->text();
        QDate date = QDate::fromString(dateStr, "dd/MM/yyyy");
        if (date.isValid()) ui->dateEdit->setDate(date);

        QString timeStr = t->item(row, 3)->text();
        QTime time = QTime::fromString(timeStr, "HH:mm");
        if (time.isValid()) ui->timeEdit_2->setTime(time);

        ui->lineEdit_45->setText(t->item(row, 4)->text());
    });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_menubtnFournisseur_clicked()
{
    ui->tabWidget_5->setCurrentIndex(0);
    refreshRdvTable();
}

void MainWindow::on_pushButton_58_clicked()
{
    if (!validateRdvInputs()) return;

    RDV r(ui->lineEdit_43->text().toInt(),
          ui->lineEdit_48->text(),
          ui->dateEdit->date(),
          ui->timeEdit_2->time(),
          ui->lineEdit_45->text().toInt());

    if (r.ajouter()) {
        QMessageBox::information(this, "Succès", "RDV ajouté !");
        refreshRdvTable();
        clearRdvForm();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec ajout.");
    }
}

void MainWindow::on_pushButton_59_clicked()
{
    if (!validateRdvInputs()) return;

    RDV r(ui->lineEdit_43->text().toInt(),
          ui->lineEdit_48->text(),
          ui->dateEdit->date(),
          ui->timeEdit_2->time(),
          ui->lineEdit_45->text().toInt());

    if (r.modifier()) {
        QMessageBox::information(this, "Succès", "RDV modifié !");
        refreshRdvTable();
    }
}

void MainWindow::on_pushButton_62_clicked()
{
    int id = ui->lineEdit_43->text().toInt();
    if (id == 0 || QMessageBox::question(this, "Confirmer", "Supprimer ?") != QMessageBox::Yes) return;
    if (rdvTmp.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Supprimé !");
        refreshRdvTable();
        clearRdvForm();
    }
}

void MainWindow::on_pushButton_61_clicked()
{
    QSqlQueryModel *model = rdvTmp.rechercher(ui->comboBox_16->currentText(), ui->lineEdit_46->text());
    populateTableFromModel(model, ui->tableWidget_8);
    delete model;
}

void MainWindow::on_pushButton_64_clicked()
{
    QString ordre = ui->radioButton_16->isChecked() ? "DESC" : "ASC";
    QSqlQueryModel *model = rdvTmp.trier(ui->comboBox_16->currentText(), ordre);
    populateTableFromModel(model, ui->tableWidget_8);
    delete model;
}

void MainWindow::on_pushButton_60_clicked() { clearRdvForm(); }

void MainWindow::on_pushButton_63_clicked() { exportToPdf(); }

void MainWindow::on_btnStatistiqueFournisseur_clicked()
{
    ui->tabWidget_5->setCurrentIndex(1);
    QSqlQueryModel *model = rdvTmp.statistiqueParJour();
    populateTableFromModel(model, ui->tableWidgetStatFournisseur);

    int total = 0;
    for (int r = 0; r < model->rowCount(); ++r) total += model->data(model->index(r, 1)).toInt();
    if (total > 0) {
        ui->tableWidgetStatFournisseur->setColumnCount(3);
        ui->tableWidgetStatFournisseur->setHorizontalHeaderLabels({"Jour", "Nb RDV", "%"});
        for (int r = 0; r < model->rowCount(); ++r) {
            double pct = model->data(model->index(r, 1)).toInt() * 100.0 / total;
            ui->tableWidgetStatFournisseur->setItem(r, 2, new QTableWidgetItem(QString::number(pct, 'f', 1) + "%"));
        }
    }
    delete model;
}

void MainWindow::refreshRdvTable()
{
    QSqlQueryModel *model = rdvTmp.afficher();
    if (model->rowCount() == 0) {
        ui->tableWidget_8->setRowCount(1);
        ui->tableWidget_8->setItem(0, 0, new QTableWidgetItem("Aucun RDV trouvé"));
        delete model;
        return;
    }
    populateTableFromModel(model, ui->tableWidget_8);
    delete model;
}

void MainWindow::populateTableFromModel(QSqlQueryModel *model, QTableWidget *table)
{
    if (!model || !table) return;
    table->clear();
    table->setRowCount(model->rowCount());
    table->setColumnCount(model->columnCount());

    for (int c = 0; c < model->columnCount(); ++c)
        table->setHorizontalHeaderItem(c, new QTableWidgetItem(model->headerData(c, Qt::Horizontal).toString()));

    for (int r = 0; r < model->rowCount(); ++r)
        for (int c = 0; c < model->columnCount(); ++c)
            table->setItem(r, c, new QTableWidgetItem(model->data(model->index(r, c)).toString()));

    table->resizeColumnsToContents();
}

bool MainWindow::validateRdvInputs()
{
    if (ui->lineEdit_43->text().isEmpty() || ui->lineEdit_48->text().isEmpty() || ui->lineEdit_45->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Tous les champs sont requis.");
        return false;
    }
    return true;
}

void MainWindow::clearRdvForm()
{
    ui->lineEdit_43->clear();
    ui->lineEdit_48->clear();
    ui->lineEdit_45->clear();
    ui->lineEdit_46->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit_2->setTime(QTime::currentTime());
}

void MainWindow::exportToPdf()
{
    QString file = QFileDialog::getSaveFileName(this, "Exporter PDF", "", "PDF (*.pdf)");
    if (file.isEmpty()) return;

    QPdfWriter pdf(file);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&pdf);
    painter.setFont(QFont("Arial", 10));

    painter.drawText(100, 100, "Liste des Rendez-vous");
    int y = 200;
    for (int r = 0; r < ui->tableWidget_8->rowCount(); ++r) {
        QString line;
        for (int c = 0; c < 5; ++c) line += ui->tableWidget_8->item(r, c)->text() + "  ";
        painter.drawText(100, y, line);
        y += 30;
    }
    painter.end();
    QMessageBox::information(this, "PDF", "Exporté !");
}

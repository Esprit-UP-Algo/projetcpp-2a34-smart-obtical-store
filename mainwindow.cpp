#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QPrinter>
#include <QFileDialog>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->tableWidget->setRowCount(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//l'ajout
void MainWindow::on_pushButton_clicked()
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(row);

    QString ref = ui->lineEdit->text();
    QString marque = ui->lineEdit_2->text();
    QString categorie = ui->comboBox->currentText();
    double prix = ui->lineEdit_3->text().toDouble();
    int stock = ui->lineEdit_4->text().toInt();

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(ref));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(marque));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(categorie));

    QTableWidgetItem *itemPrix = new QTableWidgetItem(QString::number(prix));
    itemPrix->setData(Qt::EditRole, prix);
    ui->tableWidget->setItem(row, 3, itemPrix);

    QTableWidgetItem *itemStock = new QTableWidgetItem(QString::number(stock));
    itemStock->setData(Qt::EditRole, stock);
    ui->tableWidget->setItem(row, 4, itemStock);

}
//modification
void MainWindow::on_pushButton_2_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Erreur", "Sélectionnez une ligne à modifier !");
        return;
    }

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(ui->lineEdit->text()));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(ui->lineEdit_2->text()));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(ui->comboBox->currentText()));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(ui->lineEdit_3->text()));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(ui->lineEdit_4->text()));
}
//renitialiser
void MainWindow::on_pushButton_3_clicked()
{

    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_9->clear();
    ui->comboBox->setCurrentIndex(0);
}
//trie
void MainWindow::on_pushButton_10_clicked()
{
    int column = (ui->comboBox_3->currentText() == "Prix") ? 3 : 4;
    Qt::SortOrder order = ui->radioButton_3->isChecked() ? Qt::DescendingOrder : Qt::AscendingOrder;
    ui->tableWidget->sortItems(column, order);

}


void MainWindow::on_pushButton_7_clicked()
{
    QString text = ui->lineEdit_9->text().trimmed();

    if (text.isEmpty()) {
        QMessageBox::warning(this, "Recherche", "Veuillez entrer un mot clé pour chercher.");
        return;
    }

    bool found = false;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        bool match = false;


        if (ui->tableWidget->item(row, 0) && ui->tableWidget->item(row, 0)->text().contains(text, Qt::CaseInsensitive)) {
            match = true;
        }
        if (ui->tableWidget->item(row, 2) && ui->tableWidget->item(row, 2)->text().contains(text, Qt::CaseInsensitive)) {
            match = true;
        }

        ui->tableWidget->setRowHidden(row, !match);

        if (match) found = true;
    }

    if (!found) {
        QMessageBox::information(this, "Recherche", "Aucun résultat trouvé.");
    }
}
// Supprimer
void MainWindow::on_pushButton_8_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row >= 0) {
        ui->tableWidget->removeRow(row);
    } else {
        QMessageBox::warning(this, "Erreur", "Sélectionnez une ligne à supprimer !");
    }
}

// PDF
void MainWindow::on_pushButton_9_clicked()
{
    if(ui->tableWidget->rowCount() == 0) {
        QMessageBox::warning(this, "Erreur", "Le tableau est vide !");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", "", "*.pdf");
    if(fileName.isEmpty())
        return;

    if(!fileName.endsWith(".pdf"))
        fileName += ".pdf";

    QString html = "<table border='1' cellspacing='0' cellpadding='2'>";

    html += "<tr>";
    for(int c = 0; c < ui->tableWidget->columnCount(); ++c) {
        html += "<th>" + ui->tableWidget->horizontalHeaderItem(c)->text() + "</th>";
    }
    html += "</tr>";

    for(int r = 0; r < ui->tableWidget->rowCount(); ++r) {
        html += "<tr>";
        for(int c = 0; c < ui->tableWidget->columnCount(); ++c) {
            QTableWidgetItem *item = ui->tableWidget->item(r, c);
            html += "<td>" + QString(item ? item->text() : "") + "</td>";
        }
        html += "</tr>";
    }

    html += "</table>";

    QPrinter printer(QPrinter::PrinterResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(this, "Succès", "PDF exporté avec succès !");
}



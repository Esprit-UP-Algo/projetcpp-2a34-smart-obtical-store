#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSqlQueryModel *model = Ftmp.afficher();
    ui->tableWidget->setRowCount(model->rowCount());
    ui->tableWidget->setColumnCount(model->columnCount());

    for (int i = 0; i < model->rowCount(); i++) {
        for (int j = 0; j < model->columnCount(); j++) {
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(model->data(model->index(i, j)).toString()));
        }
    }



}

MainWindow::~MainWindow()
{
    delete ui;
}

// === AJOUT ===
void MainWindow::on_pushButton_clicked()
{
    int id = ui->lineEdit->text().toInt();
    QString nom = ui->lineEdit_5->text();
    QString adresse = ui->lineEdit_2->text();
    QString email = ui->lineEdit_3->text();
    QString tel = ui->lineEdit_4->text();
    QString specialite = ui->comboBox->currentText();

    Fournisseur F(id, nom, adresse, email, tel, specialite);

    bool test = F.ajouter();
    if (test) {
        QMessageBox::information(this, "Ajout", "Fournisseur ajouté avec succès");
        QSqlQueryModel *model = Ftmp.afficher();
        ui->tableWidget->setRowCount(model->rowCount());
        ui->tableWidget->setColumnCount(model->columnCount());

        for (int i = 0; i < model->rowCount(); i++) {
            for (int j = 0; j < model->columnCount(); j++) {
                ui->tableWidget->setItem(i, j, new QTableWidgetItem(model->data(model->index(i, j)).toString()));
            }
        }

    } else {
        QMessageBox::warning(this, "Erreur", "Échec de l’ajout");
    }
}

// === SUPPRESSION ===
void MainWindow::on_pushButton_8_clicked()
{
    int id = ui->lineEdit->text().toInt();
    bool test = Ftmp.supprimer(id);

    if (test) {
        QMessageBox::information(this, "Suppression", "Fournisseur supprimé");
        QSqlQueryModel *model = Ftmp.afficher();
        ui->tableWidget->setRowCount(model->rowCount());
        ui->tableWidget->setColumnCount(model->columnCount());

        for (int i = 0; i < model->rowCount(); i++) {
            for (int j = 0; j < model->columnCount(); j++) {
                ui->tableWidget->setItem(i, j, new QTableWidgetItem(model->data(model->index(i, j)).toString()));
            }
        }

    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la suppression");
    }
}

// === MODIFICATION ===
void MainWindow::on_pushButton_2_clicked()
{
    int id = ui->lineEdit->text().toInt();
    QString nom = ui->lineEdit_5->text();
    QString adresse = ui->lineEdit_2->text();
    QString email = ui->lineEdit_3->text();
    QString tel = ui->lineEdit_4->text();
    QString specialite = ui->comboBox->currentText();

    Fournisseur F(id, nom, adresse, email, tel, specialite);

    bool test = F.modifier();
    if (test) {
        QMessageBox::information(this, "Modification", "Fournisseur modifié");
        QSqlQueryModel *model = Ftmp.afficher();
        ui->tableWidget->setRowCount(model->rowCount());
        ui->tableWidget->setColumnCount(model->columnCount());

        for (int i = 0; i < model->rowCount(); i++) {
            for (int j = 0; j < model->columnCount(); j++) {
                ui->tableWidget->setItem(i, j, new QTableWidgetItem(model->data(model->index(i, j)).toString()));
            }
        }

    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la modification");
    }
}


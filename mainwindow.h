#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fournisseur.h"
#include <QSqlQueryModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();   // Ajouter
    void on_pushButton_2_clicked(); // Modifier
    void on_pushButton_3_clicked(); // Annuler
    void on_pushButton_8_clicked(); // Supprimer

    void on_pushButton_7_clicked(); // Rechercher
    void on_pushButton_10_clicked(); // Trier
    void on_pushButton_9_clicked(); // Exporter PDF

private:
    Ui::MainWindow *ui;
    Fournisseur Ftmp;

    // Helpers
    void remplirTable(QSqlQueryModel *model);
    bool controleSaisie(); // validation stricte demandée
    QString buildFilterFromSearch(const QString &term);
    QString currentOrderBy(); // construit ORDER BY selon comboBox_3 + radio buttons
};
#endif // MAINWINDOW_H

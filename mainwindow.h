#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "messagerie.h"
#include <QMainWindow>
#include <QSqlQueryModel>
#include "employee.h"
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setIdEmployeConnecte(int id) { idEmployeConnecte = id; }

private slots:
    // Button click handlers
    void on_pushButton_ajouter_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_rechercher_clicked();
    void on_pushButton_afficher_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_messagerie_clicked();

    // Table selection handler
    void on_tableWidget_cellClicked(int row, int column);

    void on_pushButton_supprimer_clicke_clicked();

    void on_pushButton_trier_clicked();

    void on_PDF_clicked();

    void on_stat_clicked();

private:
    Ui::MainWindow *ui;

    // Helper methods

    void displayInTableWidget(QSqlQueryModel* model);
    void refreshEmployeeTable();
    void clearInputFields();
    void loadEmployeeToForm(int id);
    int idEmployeConnecte;
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rdv.h"
#include <QTableWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_menubtnFournisseur_clicked();
    void on_pushButton_58_clicked();
    void on_pushButton_59_clicked();
    void on_pushButton_60_clicked();
    void on_pushButton_62_clicked();
    void on_pushButton_61_clicked();
    void on_pushButton_64_clicked();
    void on_pushButton_63_clicked();
    void on_btnStatistiqueFournisseur_clicked();

private:
    Ui::MainWindow *ui;
    RDV rdvTmp;

    void refreshRdvTable();
    void populateTableFromModel(QSqlQueryModel *model, QTableWidget *table);
    bool validateRdvInputs();
    void clearRdvForm();
    void exportToPdf();
};

#endif // MAINWINDOW_H

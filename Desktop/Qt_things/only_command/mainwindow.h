#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDate>
#include "connection.h"
#include "add.h"
#include "update.h"
#include "find.h"
#include "ui_mainwindow.h"        // ← THIS LINE FIXES ALL WARNINGS!

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
    void on_add_clicked();
    void on_update_clicked();
    void on_cancel_clicked();
    void on_recherchebutton_clicked();
    void on_pushButton_10_clicked();
    void on_tab_show_command_cellClicked(int row, int column);
    void on_supprimer_clicked();
    //void on_imprimer_clicked();
private:
    Ui::MainWindow *ui;
    Connection conn;

    void chargerTableau();
    void viderFormulaire();
};

#endif // MAINWINDOW_H

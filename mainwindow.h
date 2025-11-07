#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fournisseur.h"

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
    void on_pushButton_clicked();   // Ajouter
    void on_pushButton_8_clicked(); // Supprimer
    void on_pushButton_2_clicked(); // Modifier

private:
    Ui::MainWindow *ui;
    Fournisseur Ftmp;
};

#endif // MAINWINDOW_H



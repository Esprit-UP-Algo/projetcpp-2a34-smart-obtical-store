#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPdfWriter>
#include <QPageSize>
#include <QPainter>
#include "fournisseur.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void remplirTable(QSqlQueryModel *model, QTableWidget *table);

private slots:
    void on_btnAjoutFournisseur_clicked();
    void on_btnModifierFournisseur_clicked();
    void on_btnSupprimerFournisseur_clicked();
    void on_btnAnnulerFournisseur_clicked();

    void on_btnRechercheFournisseur_clicked();
    void on_btnTriFournisseur_clicked();
    void on_btnExporterPDFFournisseur_clicked();
    void on_btnStatistiqueFournisseur_clicked();

private:
    Ui::MainWindow *ui;
    Fournisseur F;
    // === AJOUTE CES DÉCLARATIONS ===
    void refreshMainTable();
    bool validateInputs(long long &outId, QString &outNom, QString &outAdresse,
                        QString &outEmail, QString &outTelephone, QString &outSpecialite,
                        QString &errMsg);
    void populateTableFromModel(QSqlQueryModel *model, QTableWidget *table);

    // Supprime cette ligne si tu n'utilises pas remplirTable
    // void remplirTable(QSqlQueryModel *model, QTableWidget *table);
};

#endif // MAINWINDOW_H

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QDateEdit>
#include <QTimeEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QRadioButton>
#include "client.h"
#include "rdv.h"
#include "produit.h"
#include <QTimer>
#include <QVBoxLayout>      // ⭐ AJOUTEZ CETTE LIGNE
#include <QLayoutItem>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation
    void afficherPageAccueil();
    void afficherPageClients();
    void afficherPageProduits();
    void afficherPageRDV();

    // === CLIENTS ===
    void ajouterClient();
    void modifierClient();
    void supprimerClient();
    void rechercherClient();
    void trierClients();
    void exporterClientsPDF();
    void afficherStatistiquesClients();
    void chargerClients();
    void selectionClientChange();
    void ajouterPointsClient();
    void afficherPieChart(int c12_18, int c19_30, int c31_50, int c50_plus, int total);
    void on_pdf_bt_clicked();
    void refreshClients();


    // === RDV ===
    void ajouterRDV();
    void modifierRDV();
    void supprimerRDV();
    void rechercherRDV();
    void trierRDV();
    void afficherStatistiquesRDV();
    void chargerRDV();
    void selectionRDVChange();

    // === PRODUITS ===
    void ajouterProduit();
    void modifierProduit();
    void supprimerProduit();
    void rechercherProduit();
    void trierProduit();
    void exporterProduitsPDF();
    void chargerProduits();
    void selectionProduitChange();

private:
    void initialiserConnections();
    void viderChampsClients();
    void viderChampsRDV();
    void viderChampsProduits();
    void rafraichirTableRDV(QSqlQueryModel *model);
    QString currentOrderBy;
    bool currentAsc;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

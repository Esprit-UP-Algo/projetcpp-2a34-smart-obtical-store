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
#include "fournisseur.h"
#include "historique.h"
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
    void afficherPageRDV();
    void afficherPageproduit();
    void afficherPagefournisseur();


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
    bool validerClient(const Client& c);


    // === RDV ===
    void ajouterRDV();
    void modifierRDV();
    void supprimerRDV();
    void rechercherRDV();
    void trierRDV();
    void afficherStatistiquesRDV();
    void chargerRDV();
    void selectionRDVChange();

    // ===================== PRODUITS =====================
    void on_btnAjouterProduit_clicked();
    void on_btnModifierProduit_clicked();
    void on_btnAnnulerProduit_clicked();        // Supprimer TOUT
    void on_btnSupprimerProduit_clicked();      // Supprimer un seul
    void on_btnRechercherProduit_clicked();
    void on_btnTrierProduit_clicked();
    void on_btnExporterProduit_clicked();
    void on_pushButton_stat_clicked();           // Statistiques produits
    void on_pushButton_anomalies_clicked();     // Anomalies produits
    void on_pushButton_remiseta_clicked();
    // Clic sur une ligne du tableau
    void on_tableWidget_cellClicked(int row, int column);

    // ===================== INTELLIGENCE ARTIFICIELLE =====================
    void afficherStatistiques();
    void detecterAnomalies();

    void updateIntelligente();

    // ===================== FOURNISSEURS =====================
    void on_btnAjouterFournisseur_clicked();
    void on_btnAnnulerFournisseur_clicked();
    void on_btnModifierFournisseur_clicked();
    void on_btnSupprimerFournisseur_clicked();
    void on_btnRechercherFournisseur_clicked();
    void on_btnTrierFournisseur_clicked();
    void on_btnStatFournisseur_clicked() ;
    void on_btnExporterFournisseur_clicked();
    void on_tableWidgetFournisseur_cellClicked(int row, int column);

    // ===================== HISTORIQUE =====================
    void on_btnHistoriqueFournisseurs_clicked();     // Bouton pour aller dans l’historique
    void on_btnHistFiltrer_clicked();               // Bouton "Filtrer"
    void on_btnHistRefresh_clicked();                // Bouton "Actualiser"
    void on_btnHistExporterPDF_clicked();            // Bouton "Exporter PDF" historique
    // ===================== COMBO "TOUS" DANS HISTORIQUE =====================
    void on_comboHistAction_currentIndexChanged(int index);
    void populateTableHistorique(QSqlQueryModel *model);

    // ===================== AUTRES BOUTONS POSSIBLES =====================
    void on_pushButton_3_clicked();   // au cas où Qt l’a nommé comme ça (Filtrer)
    void on_pushButton_4_clicked();   // Actualiser
    void on_pushButton_5_clicked();   // Exporter PDF

private:
    void initialiserConnections();
    void viderChampsClients();
    void viderChampsRDV();
    void viderChampsproduit();
    void viderChampsfournisseur();
    void rafraichirTableRDV(QSqlQueryModel *model);
    // Fonctions utilitaires
    void afficherProduits();
    void refreshFournisseurs();
    void refreshHistorique();
    void appliquerRemiseAuto();
    void testHistoriqueForce();   // ← AJOUTE ÇA
    QString currentOrderBy;
    bool currentAsc;

    Ui::MainWindow *ui;
    Produit prod;
    Fournisseur four;
    Historique hist;
    QTimer *timer;
};

#endif // MAINWINDOW_H

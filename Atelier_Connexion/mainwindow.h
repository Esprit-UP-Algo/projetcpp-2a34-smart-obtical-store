#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QFile>
#include <QDir>
#include <QPixmap>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCoreApplication>

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
    // Clients
    void on_ajouter_bt_clicked();
    void on_modifier_bt_clicked();
    void on_supprimer_bt_clicked();

    void on_tableWidget_cl_itemClicked(QTableWidgetItem *item);
    void on_tableWidget_cl_currentCellChanged(int currentRow, int currentColumn,
                                              int previousRow, int previousColumn);

    void on_trier_bt_clicked();
    void on_radioCroissant_toggled(bool checked);
    void on_radioDecroissant_toggled(bool checked);
    void on_recherche_textChanged(const QString &);
    void on_comboBox_2_currentIndexChanged(int);
    void afficherPieChart(int c12_18, int c19_30, int c31_50, int c50_plus, int total);

    void on_pdf_bt_clicked();
    void generateQR();
    void saveQR();
    void copyQR();


private:
    Ui::MainWindow *ui;
    int currentClientId;
    QString currentOrderBy;
    bool currentAsc;

    bool readForm(int &id, QString &nom, QString &prenom, QString &mail,
                  QString &tel, int &age, QString &adresse, int &points);
    void refreshClients();
    void showRecommendationForCurrentClient();
    QString imagesBasePath() const;
    void afficherStatistiquesPie();
};

#endif // MAINWINDOW_H


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QSqlQueryModel>
// ⭐ Includes pour le graphique et PDF - DANS CET ORDRE PRÉCIS ⭐
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QtPrintSupport/QPrinter>
#include <QtGui/QPageLayout>
#include <QtGui/QPageSize>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QDate>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>
#include <QPieSeries>
#include <QChart>
#include <QChartView>
#include <QMessageBox>
#include <QDesktopServices>   // ← AJOUTE ÇA
#include <QUrl>
#include <QPdfWriter>
#include <QPainter>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initialiserConnections();

    // Charger les données initiales
    chargerClients();
    chargerRDV();
    refreshFournisseurs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialiserConnections()
{
    // ==================== NAVIGATION ====================
    // Menu principal (page_5)


    // Menu RDV (page)
    connect(ui->acc, &QPushButton::clicked, this, &MainWindow::afficherPageAccueil);
    connect(ui->menubtnFournisseur, &QPushButton::clicked, this, &MainWindow::afficherPageRDV);
    connect(ui->client, &QPushButton::clicked, this, &MainWindow::afficherPageClients);
    connect(ui->btnMenuProduit, &QPushButton::clicked, this, &MainWindow::afficherPageproduit);
    connect(ui->btnMenuFournisseur, &QPushButton::clicked, this, &MainWindow::afficherPagefournisseur);

    // Menu Clients (page_2)
    connect(ui->pushButton_110, &QPushButton::clicked, this, &MainWindow::afficherPageAccueil);
    connect(ui->rdv_bt, &QPushButton::clicked, this, &MainWindow::afficherPageRDV);
    connect(ui->client_bt, &QPushButton::clicked, this, &MainWindow::afficherPageClients);
    connect(ui->produit_bt, &QPushButton::clicked, this, &MainWindow::afficherPageproduit);
    connect(ui->fournisseur_bt, &QPushButton::clicked, this, &MainWindow::afficherPagefournisseur);
    connect(ui->id_client, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text.length() > 6) {
            ui->id_client->setText(text.left(6));
        }
        // Accepter seulement les chiffres
        QString filtered = text;
        filtered.remove(QRegularExpression("[^0-9]"));
        if (filtered != text) {
            ui->id_client->setText(filtered);
        }
    });

    // Âge : maximum 3 chiffres
    connect(ui->age_client, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text.length() > 3) {
            ui->age_client->setText(text.left(3));
        }
        QString filtered = text;
        filtered.remove(QRegularExpression("[^0-9]"));
        if (filtered != text) {
            ui->age_client->setText(filtered);
        }
    });

    // Téléphone : exactement 8 chiffres
    connect(ui->telephone_client, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text.length() > 8) {
            ui->telephone_client->setText(text.left(8));
        }
        QString filtered = text;
        filtered.remove(QRegularExpression("[^0-9]"));
        if (filtered != text) {
            ui->telephone_client->setText(filtered);
        }
    });

    // Points : maximum 4 chiffres
    connect(ui->points_client, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text.length() > 4) {
            ui->points_client->setText(text.left(4));
        }
        QString filtered = text;
        filtered.remove(QRegularExpression("[^0-9]"));
        if (filtered != text) {
            ui->points_client->setText(filtered);
        }
    });


    // Menu fournisseur (page_3)
    connect(ui->pushButton_111, &QPushButton::clicked, this, &MainWindow::afficherPageAccueil);
    connect(ui->rdv_bt_2, &QPushButton::clicked, this, &MainWindow::afficherPageRDV);
    connect(ui->fournisseur_bt_2, &QPushButton::clicked, this, &MainWindow::afficherPageClients);
    connect(ui->produit_bt_2, &QPushButton::clicked, this, &MainWindow::afficherPageproduit);
    connect(ui->pushButtonFournisseur, &QPushButton::clicked, this, &MainWindow::afficherPagefournisseur);


    // Menu produit (page_4)
    connect(ui->acc_2, &QPushButton::clicked, this, &MainWindow::afficherPageAccueil);
    connect(ui->btnMenuProduit_2, &QPushButton::clicked, this, &MainWindow::afficherPageRDV);
    connect(ui->client_2, &QPushButton::clicked, this, &MainWindow::afficherPageClients);
    connect(ui->pushButtonProduit, &QPushButton::clicked, this, &MainWindow::afficherPageproduit);
    connect(ui->btnMenuFournisseur_2, &QPushButton::clicked, this, &MainWindow::afficherPagefournisseur);
    connect(ui->tableWidget_2, &QTableWidget::cellClicked,
            this, &MainWindow::on_tableWidget_cellClicked);


    // ==================== CLIENTS ====================
    connect(ui->ajouter_bt, &QPushButton::clicked, this, &MainWindow::ajouterClient);
    connect(ui->modifier_bt, &QPushButton::clicked, this, &MainWindow::modifierClient);
    connect(ui->annuler_bt, &QPushButton::clicked, this, &MainWindow::supprimerClient);
    connect(ui->trier_bt, &QPushButton::clicked, this, &MainWindow::trierClients);
    connect(ui->pdf_bt, &QPushButton::clicked, this, &MainWindow::exporterClientsPDF);
    connect(ui->supprimer_bt, &QPushButton::clicked, this, &MainWindow::supprimerClient);
    connect(ui->tabWidget_13, &QTabWidget::currentChanged, this, [this](int index) {
        if (index == 1) {  // Onglet statistiques
            QTimer::singleShot(100, this, &MainWindow::refreshClients);
        }
    });

    // Recherche clients
    connect(ui->recherche, &QLineEdit::returnPressed, this, &MainWindow::rechercherClient);

    // Sélection table clients
    connect(ui->tableWidget_cl, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::selectionClientChange);


    // ==================== RDV ====================
    connect(ui->pushButton_58, &QPushButton::clicked, this, &MainWindow::ajouterRDV);
    connect(ui->pushButton_59, &QPushButton::clicked, this, &MainWindow::modifierRDV);
    connect(ui->pushButton_60, &QPushButton::clicked, this, &MainWindow::supprimerRDV);
    connect(ui->pushButton_64, &QPushButton::clicked, this, &MainWindow::trierRDV);
    connect(ui->pushButton_62, &QPushButton::clicked, this, &MainWindow::supprimerRDV);
    connect(ui->pushButton_61, &QPushButton::clicked, this, &MainWindow::rechercherRDV);
    connect(ui->btnStatistiqueFournisseur, &QPushButton::clicked, this, &MainWindow::afficherStatistiquesRDV);
    // Sélection table RDV
    connect(ui->tableWidget_8, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::selectionRDVChange);


}

// ==================== NAVIGATION ====================
void MainWindow::afficherPageAccueil() {
    ui->stackedWidget->setCurrentWidget(ui->page_5);
}

void MainWindow::afficherPageClients() {
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    chargerClients();
}

void MainWindow::afficherPageRDV() {
    ui->stackedWidget->setCurrentWidget(ui->page);
    chargerRDV();
}
void MainWindow::afficherPagefournisseur() {
    ui->stackedWidget->setCurrentWidget(ui->page_3);
    refreshFournisseurs();
}

void MainWindow::afficherPageproduit() {
    ui->stackedWidget->setCurrentWidget(ui->page_4);
    afficherProduits();
    detecterAnomalies();
    appliquerRemiseAuto();
}
// ==================== FONCTIONS CLIENTS ====================
bool MainWindow::validerClient(const Client& c) {
    if (!c.isValidId()) {
        QMessageBox::warning(this, "❌ Erreur de validation",
                             "L'ID doit contenir exactement 6 chiffres !\n\n"
                             "Exemple : 100001, 100002, etc.");
        return false;
    }

    if (!c.isValidAge()) {
        QMessageBox::warning(this, "❌ Erreur de validation",
                             "L'âge doit être entre 1 et 999 ans !");
        return false;
    }

    if (!c.isValidPoints()) {
        QMessageBox::warning(this, "❌ Erreur de validation",
                             "Les points de fidélité doivent être entre 0 et 9999 !");
        return false;
    }

    if (!c.isValidTelephone()) {
        QMessageBox::warning(this, "❌ Erreur de validation",
                             "Le téléphone doit contenir exactement 8 chiffres !\n\n"
                             "Exemple : 50123456, 98765432");
        return false;
    }

    if (!c.isValidEmail()) {
        QMessageBox::warning(this, "❌ Erreur de validation",
                             "L'email est invalide !\n\n"
                             "Format attendu : exemple@domaine.com");
        return false;
    }

    return true;
}
void MainWindow::ajouterClient()
{
    // Créer l'objet client avec les données du formulaire
    Client c;
    c.setId(ui->id_client->text().toInt());
    c.setNom(ui->nom_client->text().trimmed());
    c.setPrenom(ui->prenom_client->text().trimmed());
    c.setEmail(ui->mail_client->text().trimmed());
    c.setTelephone(ui->telephone_client->text().trimmed());
    c.setAge(ui->age_client->text().toInt());
    c.setAdresse(ui->adresse_client->text().trimmed());
    c.setPointsFidelite(ui->points_client->text().toInt());

    // ✅ VALIDATION AVANT AJOUT
    if (!validerClient(c)) {
        return; // Arrêter si validation échoue
    }

    // ✅ Vérifier que les champs texte ne sont pas vides
    if (c.getNom().isEmpty() || c.getPrenom().isEmpty()) {
        QMessageBox::warning(this, "❌ Champs obligatoires",
                             "Le nom et le prénom sont obligatoires !");
        return;
    }

    // Ajout dans la base de données
    QString errMsg;
    if (c.ajouter(&errMsg)) {
        QMessageBox::information(this, "✅ Succès",
                                 "Client ajouté avec succès !");
        viderChampsClients();
        chargerClients();
    } else {
        QMessageBox::critical(this, "❌ Erreur",
                              "Échec de l'ajout du client :\n" + errMsg);
    }
}
void MainWindow::modifierClient()
{
    if (ui->id_client->text().isEmpty()) {
        QMessageBox::warning(this, "⚠️ Attention",
                             "Veuillez sélectionner un client à modifier !");
        return;
    }

    // Créer l'objet client
    Client c;
    c.setId(ui->id_client->text().toInt());
    c.setNom(ui->nom_client->text().trimmed());
    c.setPrenom(ui->prenom_client->text().trimmed());
    c.setEmail(ui->mail_client->text().trimmed());
    c.setTelephone(ui->telephone_client->text().trimmed());
    c.setAge(ui->age_client->text().toInt());
    c.setAdresse(ui->adresse_client->text().trimmed());
    c.setPointsFidelite(ui->points_client->text().toInt());

    // ✅ VALIDATION AVANT MODIFICATION
    if (!validerClient(c)) {
        return;
    }

    if (c.getNom().isEmpty() || c.getPrenom().isEmpty()) {
        QMessageBox::warning(this, "❌ Champs obligatoires",
                             "Le nom et le prénom sont obligatoires !");
        return;
    }

    // Modification dans la base de données
    QString errMsg;
    if (c.modifier(&errMsg)) {
        QMessageBox::information(this, "✅ Succès",
                                 "Client modifié avec succès !");
        viderChampsClients();
        chargerClients();
    } else {
        QMessageBox::critical(this, "❌ Erreur",
                              "Échec de la modification :\n" + errMsg);
    }
}

void MainWindow::supprimerClient()
{
    if (ui->id_client->text().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un client à supprimer");
        return;
    }

    auto reponse = QMessageBox::question(this, "Confirmation",
                                         "Voulez-vous vraiment supprimer ce client?",
                                         QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes) {
        QString errMsg;
        if (Client::supprimer(ui->id_client->text().toInt(), &errMsg)) {
            QMessageBox::information(this, "Succès", "Client supprimé!");
            viderChampsClients();
            chargerClients();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec suppression: " + errMsg);
        }
    }
}
void MainWindow::on_pdf_bt_clicked()
{
    if(ui->tableWidget_cl->rowCount() == 0){
        QMessageBox::warning(this, "PDF", "Aucun client à exporter !");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer le PDF",
                                                    QDir::homePath() + "/clients_export.pdf",
                                                    "Fichiers PDF (*.pdf)");
    if(fileName.isEmpty()) return;
    if(!fileName.endsWith(".pdf", Qt::CaseInsensitive))
        fileName += ".pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF !");
        return;
    }

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // ============= FONCTION: Fond de page =============
    auto drawBackground = [&]() {
        QLinearGradient bgGradient(0, 0, 0, printer.height());
        bgGradient.setColorAt(0.0, QColor("#FFF8E1"));
        bgGradient.setColorAt(0.3, QColor("#FFE0B2"));
        bgGradient.setColorAt(0.7, QColor("#FFCC80"));
        bgGradient.setColorAt(1.0, QColor("#FFB74D"));
        painter.fillRect(0, 0, printer.width(), printer.height(), bgGradient);

        painter.setPen(QPen(QColor("#E65100"), 15));
        painter.setBrush(Qt::NoBrush);
        painter.drawRoundedRect(50, 50, printer.width() - 100, printer.height() - 100, 40, 40);
    };

    // ============= FONCTION: En-tête page =============
    auto drawPageHeader = [&](int &y) {
        // Logo
        painter.setBrush(QColor("#FF6F00"));
        painter.setPen(QPen(QColor("#E65100"), 10));
        painter.drawEllipse(printer.width()/2 - 80, y - 100, 160, 160);
        painter.setFont(QFont("Segoe UI", 48, QFont::Bold));
        painter.setPen(Qt::white);
        painter.drawText(printer.width()/2 - 80, y - 80, 160, 140, Qt::AlignCenter, "📋");

        // Titre
        painter.setFont(QFont("Georgia", 42, QFont::Bold));
        painter.setPen(QColor("#E65100"));
        painter.drawText(0, y + 20, printer.width(), 100, Qt::AlignCenter, "Liste des Clients");

        y += 140;

        // Ligne séparatrice
        painter.setPen(QPen(QColor("#FF6F00"), 8));
        painter.drawLine(200, y, printer.width() - 200, y);
        y += 60;

        // Date actuelle
        QString dateStr = "📅 " + QDate::currentDate().toString("dddd dd MMMM yyyy");
        painter.setFont(QFont("Segoe UI", 18, QFont::Bold));
        painter.setBrush(QColor("#FFECB3"));
        painter.setPen(QPen(QColor("#FF8A65"), 4));
        painter.drawRoundedRect(printer.width()/2 - 300, y, 600, 70, 20, 20);
        painter.setPen(QColor("#BF360C"));
        painter.drawText(printer.width()/2 - 280, y + 10, 560, 50, Qt::AlignCenter, dateStr);

        y += 120;
    };

    // ============= FONCTION: En-tête du tableau =============
    auto drawTableHeader = [&](int y, const QVector<int> &colWidths) {
        QLinearGradient headerGradient(0, y, 0, y + 95);
        headerGradient.setColorAt(0.0, QColor("#FF8A65"));
        headerGradient.setColorAt(1.0, QColor("#FF5722"));
        painter.setBrush(headerGradient);
        painter.setPen(QPen(QColor("#BF360C"), 6));
        painter.drawRoundedRect(80, y, printer.width() - 160, 95, 25, 25);

        painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
        painter.setPen(Qt::white);
        QStringList headers = {"ID", "Nom", "Prénom", "Email", "Téléphone", "Âge", "Adresse", "Points"};

        int xPos = 95;
        for(int j = 0; j < headers.size(); ++j) {
            painter.drawText(xPos + 5, y + 20, colWidths[j] - 10, 65, Qt::AlignCenter, headers[j]);
            xPos += colWidths[j];
        }

        return y + 115;
    };

    // ============= FONCTION: Footer =============
    auto drawFooter = [&]() {
        int footerY = printer.height() - 280;
        painter.setBrush(QColor("#FF8A65"));
        painter.setPen(QPen(QColor("#E65100"), 8));
        painter.drawRoundedRect(100, footerY, printer.width() - 200, 180, 35, 35);
        painter.setFont(QFont("Segoe UI", 18, QFont::Bold));
        painter.setPen(Qt::white);
        painter.drawText(0, footerY + 70, printer.width(), 50, Qt::AlignCenter, "Document Généré Automatiquement");
    };

    // ============= FONCTION: Raccourcir texte =============
    auto elideText = [&](const QString &text, int maxWidth, const QFontMetrics &fm) -> QString {
        if (fm.horizontalAdvance(text) <= maxWidth) return text;
        return fm.elidedText(text, Qt::ElideRight, maxWidth);
    };

    // ============= DEBUT DU DESSIN =============
    drawBackground();
    int y = 180;
    drawPageHeader(y);

    // Largeurs de colonnes
    int totalWidth = printer.width() - 200;
    QVector<int> colWidths = {
        int(totalWidth * 0.10), // ID
        int(totalWidth * 0.12), // Nom
        int(totalWidth * 0.12), // Prénom
        int(totalWidth * 0.20), // Email
        int(totalWidth * 0.13), // Téléphone
        int(totalWidth * 0.08), // Âge
        int(totalWidth * 0.16), // Adresse
        int(totalWidth * 0.09)  // Points
    };

    y = drawTableHeader(y, colWidths);

    // Style du texte des données
    QFont dataFont("Segoe UI", 10);
    painter.setFont(dataFont);
    QFontMetrics fm(dataFont);

    int totalRows = ui->tableWidget_cl->rowCount();

    for(int i = 0; i < totalRows; ++i) {
        // Saut de page si pas assez de place
        if (y > printer.height() - 550) {
            drawFooter();
            printer.newPage();
            drawBackground();
            y = 180;
            drawPageHeader(y);
            y = drawTableHeader(y, colWidths);
        }

        // Fond de la ligne
        QColor rowColor = (i % 2 == 0) ? QColor("#FFFFFF") : QColor("#FFF3E0");
        painter.setBrush(rowColor);
        painter.setPen(QPen(QColor("#FFCC80"), 3));
        painter.drawRoundedRect(80, y, printer.width() - 160, 80, 18, 18);

        // Barre latérale gauche
        painter.setBrush(QColor("#FF6F00"));
        painter.drawRoundedRect(80, y, 10, 80, 5, 5);

        // Contenu des cellules
        int xPos = 95;
        for(int j = 0; j < 8; ++j) {
            QTableWidgetItem *item = ui->tableWidget_cl->item(i, j);
            QString text = item ? item->text() : "";

            // Raccourcir si trop long
            text = elideText(text, colWidths[j] - 25, fm);

            // Alignement
            Qt::Alignment align = Qt::AlignLeft | Qt::AlignVCenter;
            if (j == 0 || j == 5 || j == 7) align = Qt::AlignCenter;

            painter.setPen(QColor("#D84315"));
            painter.drawText(xPos + 10, y + 15, colWidths[j] - 20, 50, align, text);
            xPos += colWidths[j];
        }

        y += 90;
    }

    drawFooter();
    painter.end();

    QMessageBox::information(this, "✅ Export réussi",
                             QString("PDF enregistré avec succès !\n\n📄 %1\n\n📊 %2 clients exportés")
                                 .arg(fileName)
                                 .arg(totalRows));
}
void MainWindow::rechercherClient()
{
    QString search = ui->recherche->text();
    QString criteria = ui->comboBox_2->currentText();

    int countRows, c12_18, c19_30, c31_50, c50_plus;
    QString errMsg;

    bool success = Client().afficher(ui->tableWidget_cl, "", true, search, criteria,
                                     countRows, c12_18, c19_30, c31_50, c50_plus, &errMsg);

    if (!success) {
        QMessageBox::critical(this, "Erreur", errMsg);
    }
}

void MainWindow::trierClients()
{
    QString tri = ui->trier_combo->currentText();
    bool asc = ui->radioCroissant->isChecked();

    QString orderBy;
    if (tri == "nom") orderBy = "NOMC";
    else if (tri == "Age") orderBy = "AGEC";
    else if (tri == "points de fidélité") orderBy = "POINTS_CLIENTS";

    int countRows, c12_18, c19_30, c31_50, c50_plus;
    QString errMsg;

    bool success = Client().afficher(ui->tableWidget_cl, orderBy, asc, "", "",
                                     countRows, c12_18, c19_30, c31_50, c50_plus, &errMsg);

    if (!success) {
        QMessageBox::critical(this, "Erreur", errMsg);
    }
}

void MainWindow::exporterClientsPDF()
{
    QString filepath = QFileDialog::getSaveFileName(this, "Exporter PDF", "", "PDF Files (*.pdf)");
    if (!filepath.isEmpty()) {
        // Implémentez l'export PDF pour les clients
        QMessageBox::information(this, "Info", "Export PDF pour clients à implémenter");
    }
}

void MainWindow::afficherStatistiquesClients()
{
    QString errMsg;
    Client::ClientStats stats = Client::getStatistiques(&errMsg);

    QString msg = QString("📊 STATISTIQUES CLIENTS\n\n"
                          "Total: %1 clients\n"
                          "12-18 ans: %2\n"
                          "19-30 ans: %3\n"
                          "31-50 ans: %4\n"
                          "50+ ans: %5\n\n"
                          "Moyenne d'âge: %.2f ans\n"
                          "Moyenne points: %6")
                      .arg(stats.total)
                      .arg(stats.c12_18)
                      .arg(stats.c19_30)
                      .arg(stats.c31_50)
                      .arg(stats.c50_plus)
                      .arg(stats.moyenneAge, 0, 'f', 2)
                      .arg(stats.moyennePoints);

    QMessageBox::information(this, "Statistiques", msg);
}

void MainWindow::chargerClients()
{
    int countRows, c12_18, c19_30, c31_50, c50_plus;
    QString errMsg;

    bool success = Client().afficher(ui->tableWidget_cl, "NOMC", true, "", "",
                                     countRows, c12_18, c19_30, c31_50, c50_plus, &errMsg);

    if (!success) {
        QMessageBox::critical(this, "Erreur", "Chargement échoué: " + errMsg);
    }
}

void MainWindow::selectionClientChange()
{
    QList<QTableWidgetItem*> items = ui->tableWidget_cl->selectedItems();
    if (!items.isEmpty()) {
        int row = items[0]->row();
        ui->id_client->setText(ui->tableWidget_cl->item(row, 0)->text());
        ui->nom_client->setText(ui->tableWidget_cl->item(row, 1)->text());
        ui->prenom_client->setText(ui->tableWidget_cl->item(row, 2)->text());
        ui->mail_client->setText(ui->tableWidget_cl->item(row, 3)->text());
        ui->telephone_client->setText(ui->tableWidget_cl->item(row, 4)->text());
        ui->age_client->setText(ui->tableWidget_cl->item(row, 5)->text());
        ui->adresse_client->setText(ui->tableWidget_cl->item(row, 6)->text());
        ui->points_client->setText(ui->tableWidget_cl->item(row, 7)->text());
    }
}

void MainWindow::ajouterPointsClient()
{
    if (ui->id_client->text().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un client d'abord");
        return;
    }

    bool ok;
    int points = QInputDialog::getInt(this, "Ajouter Points",
                                      "Points à ajouter:", 10, 1, 1000, 1, &ok);
    if (ok) {
        QString errMsg;
        if (Client::ajouterPoints(ui->id_client->text().toInt(), points, &errMsg)) {
            QMessageBox::information(this, "Succès", QString("%1 points ajoutés!").arg(points));
            chargerClients();
            selectionClientChange();
        } else {
            QMessageBox::critical(this, "Erreur", errMsg);
        }
    }
}
void MainWindow::afficherPieChart(int c12_18, int c19_30, int c31_50, int c50_plus, int total)
{
    // ✅ VÉRIFICATIONS
    if (!ui->tabWidget_13) {
        qDebug() << "❌ tabWidget_13 est NULL !";
        return;
    }

    QWidget *statsTab = ui->tabWidget_13->widget(1);
    if (!statsTab) {
        qDebug() << "❌ L'onglet statistiques (index 1) n'existe pas !";
        return;
    }

    // ✅ NETTOYAGE COMPLET
    QLayout *oldLayout = statsTab->layout();
    if (oldLayout) {
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    if (auto old = statsTab->findChild<QLabel*>("pieChartLabel")) {
        old->deleteLater();
    }

    // ✅ CAS SPÉCIAL: Aucun client
    if (total == 0) {
        QVBoxLayout *layout = new QVBoxLayout(statsTab);
        layout->setAlignment(Qt::AlignCenter);

        QLabel *lbl = new QLabel("📊 Aucun client dans la base de données");
        lbl->setObjectName("pieChartLabel");
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet(
            "font-size: 28px; "
            "color: #c0392b; "
            "font-weight: bold; "
            "background-color: rgba(255, 255, 255, 200); "
            "border-radius: 15px; "
            "padding: 30px;"
            );

        layout->addWidget(lbl);
        return;
    }

    // ✅ GÉNÉRATION DU PIE CHART (même taille que votre image)
    const int size = 380; // ✅ Taille optimale
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    QRectF rect(30, 30, size - 60, size - 60);

    QVector<QColor> colors = {
        QColor("#e74c3c"),  // Rouge
        QColor("#3498db"),  // Bleu
        QColor("#2ecc71"),  // Vert
        QColor("#f39c12")   // Orange
    };

    QVector<int> values = {c12_18, c19_30, c31_50, c50_plus};
    QVector<QString> labels = {"12-18 ans", "19-30 ans", "31-50 ans", "50+ ans"};

    // ✅ CALCUL PRÉCIS DES ANGLES
    int startAngle = 90 * 16;
    QVector<int> spanAngles;
    int totalAngle = 0;

    for (int i = 0; i < 4; ++i) {
        if (i < 3) {
            int angle = qRound(360.0 * 16 * values[i] / total);
            spanAngles.append(angle);
            totalAngle += angle;
        } else {
            spanAngles.append(360 * 16 - totalAngle);
        }
    }

    // ✅ DESSINER LES TRANCHES
    for (int i = 0; i < 4; ++i) {
        if (values[i] == 0) continue;

        int spanAngle = spanAngles[i];
        int percent = qRound(100.0 * values[i] / total);

        painter.setBrush(colors[i]);
        painter.setPen(QPen(Qt::white, 4)); // Bordure blanche plus épaisse
        painter.drawPie(rect, startAngle, spanAngle);

        // Position du texte
        double midAngle = (startAngle + spanAngle / 2.0) / 16.0;
        double rad = qDegreesToRadians(midAngle);
        double radius = (size / 2.0) * 0.58;

        int cx = size / 2 + radius * qCos(rad);
        int cy = size / 2 - radius * qSin(rad);

        // Label
        painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
        painter.setPen(QPen(Qt::black, 2));
        painter.drawText(cx - 65, cy - 13, 130, 26, Qt::AlignCenter, labels[i]);
        painter.setPen(Qt::white);
        painter.drawText(cx - 65, cy - 13, 130, 26, Qt::AlignCenter, labels[i]);

        // Pourcentage
        painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
        painter.setPen(QPen(Qt::black, 2));
        painter.drawText(cx - 45, cy + 13, 90, 30, Qt::AlignCenter,
                         QString("%1%").arg(percent));
        painter.setPen(Qt::white);
        painter.drawText(cx - 45, cy + 13, 90, 30, Qt::AlignCenter,
                         QString("%1%").arg(percent));

        startAngle += spanAngle;
    }

    // Bordure circulaire
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor("#34495e"), 3));
    painter.drawEllipse(rect);

    // ✅✅✅ LAYOUT GRID POUR CENTRAGE PARFAIT ✅✅✅
    QGridLayout *gridLayout = new QGridLayout(statsTab);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->setSpacing(0);

    // ✅ WIDGET CONTENEUR pour tout le contenu
    QWidget *contentWidget = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(contentWidget);
    contentLayout->setSpacing(8);
    contentLayout->setContentsMargins(0, 0, 0, 0);

    // Titre
    QLabel *titre = new QLabel("📊 Répartition par tranche d'âge");
    titre->setAlignment(Qt::AlignCenter);
    titre->setStyleSheet(
        "font-size: 17px; "
        "font-weight: bold; "
        "color: #2c3e50;"
        );
    contentLayout->addWidget(titre);

    // Le graphique
    QLabel *label = new QLabel();
    label->setObjectName("pieChartLabel");
    label->setPixmap(pixmap);
    label->setFixedSize(size, size);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "background-color: white; "
        "border: 2px solid #bdc3c7; "
        "border-radius: 15px; "
        "padding: 8px;"
        );
    contentLayout->addWidget(label);

    // Légende
    QHBoxLayout *legendLayout = new QHBoxLayout();
    legendLayout->setSpacing(12);
    legendLayout->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < 4; ++i) {
        if (values[i] == 0) continue;

        QLabel *legendItem = new QLabel();
        int percent = qRound(100.0 * values[i] / total);

        QString legendText = QString(
                                 "<div style='text-align:center;'>"
                                 "<div style='background:%1; width:28px; height:28px; "
                                 "border-radius:14px; margin:0 auto 2px auto; border:2px solid white;'></div>"
                                 "<span style='font-size:10px; font-weight:bold;'>%2</span><br>"
                                 "<span style='font-size:13px; font-weight:bold;'>%3%</span><br>"
                                 "<span style='color:#7f8c8d; font-size:9px;'>(%4)</span>"
                                 "</div>"
                                 ).arg(colors[i].name())
                                 .arg(labels[i])
                                 .arg(percent)
                                 .arg(values[i]);

        legendItem->setText(legendText);
        legendLayout->addWidget(legendItem);
    }

    contentLayout->addLayout(legendLayout);

    // ✅ AJOUTER LE CONTENEUR AU CENTRE DU GRID
    gridLayout->addWidget(contentWidget, 0, 0, Qt::AlignCenter);

    qDebug() << "✅ Pie Chart centré au milieu parfait !";
}
void MainWindow::refreshClients()
{
    int rows = 0, c12_18 = 0, c19_30 = 0, c31_50 = 0, c50_plus = 0;
    QString err;

    bool ok = Client::afficher(ui->tableWidget_cl,
                               currentOrderBy,
                               currentAsc,
                               ui->recherche->text().trimmed(),
                               ui->comboBox_2->currentText().trimmed(),
                               rows, c12_18, c19_30, c31_50, c50_plus,
                               &err);

    if (!ok) {
        QMessageBox::critical(this, "Erreur", err);
        return;
    }

    // ✅ Toujours mettre à jour le graphique
    afficherPieChart(c12_18, c19_30, c31_50, c50_plus, rows);
}

// ==================== FONCTIONS RDV ====================
void MainWindow::ajouterRDV()
{
    QString idText = ui->lineEdit_43->text().trimmed();
    QString clientText = ui->lineEdit_45->text().trimmed();
    QString desc = ui->lineEdit_48->text().trimmed();

    // تحقق من أن الحقول مش فارغة
    if (idText.isEmpty() || clientText.isEmpty() || desc.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Tous les champs sont obligatoires !");
        return;
    }

    bool ok1, ok2;
    int id_r = idText.toInt(&ok1);
    int id_client = clientText.toInt(&ok2);

    // تحقق من أن الـ ID فعلاً أرقام
    if (!ok1 || !ok2) {
        QMessageBox::warning(this, "Erreur", "L'ID RDV et l'ID Client doivent être des nombres valides !");
        return;
    }

    RDV rdv;
    rdv.setId(id_r);
    rdv.setDescription(desc);
    rdv.setDate(ui->dateEdit->date());
    rdv.setHeure(ui->timeEdit_2->time());
    rdv.setIdClient(id_client);

    if (rdv.ajouter()) {
        QMessageBox::information(this, "Succès", "RDV ajouté avec succès !");
        viderChampsRDV();
        chargerRDV();
    } else {
        QMessageBox::critical(this, "Erreur",
                              "Échec d'ajout du RDV.\n"
                              "→ Vérifiez que :\n"
                              "   • L'ID RDV n'existe pas déjà\n"
                              "   • L'ID Client existe dans le tableau Clients (ex: 100100, 100020...)\n"
                              "   • Pas de caractères spéciaux");
    }
}
void MainWindow::modifierRDV()
{
    if (ui->lineEdit_43->text().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un RDV à modifier d'abord !");
        return;
    }

    RDV rdv;
    rdv.setId(ui->lineEdit_43->text().toInt());
    rdv.setDescription(ui->lineEdit_48->text().trimmed());
    rdv.setDate(ui->dateEdit->date());
    rdv.setHeure(ui->timeEdit_2->time());
    rdv.setIdClient(ui->lineEdit_45->text().toInt());

    if (rdv.modifier()) {
        QMessageBox::information(this, "Succès", "RDV modifié avec succès !");
        viderChampsRDV();
        chargerRDV();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de modification du RDV.");
    }
}

void MainWindow::supprimerRDV()
{
    if (ui->lineEdit_43->text().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un RDV à supprimer !");
        return;
    }

    int id = ui->lineEdit_43->text().toInt();
    auto reponse = QMessageBox::question(this, "Confirmation",
                                         QString("Supprimer le RDV N° %1 ?").arg(id),
                                         QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes) {
        if (RDV::supprimer(id)) {
            QMessageBox::information(this, "Succès", "RDV supprimé avec succès !");
            viderChampsRDV();
            chargerRDV();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de suppression du RDV.");
        }
    }
}

void MainWindow::rechercherRDV()
{
    QString critere = ui->comboBox_16->currentText();
    QString valeur = ui->lineEdit_46->text().trimmed();

    QSqlQueryModel *model = RDV::rechercher(critere, valeur);
    rafraichirTableRDV(model);
    delete model; // مهم! نتجنب memory leak
}

void MainWindow::trierRDV()
{
    QString critere = ui->comboBox_16->currentText();
    QString ordre = ui->radioButton_15->isChecked() ? "ASC" : "DESC";

    QSqlQueryModel *model = RDV::trier(critere, ordre);
    rafraichirTableRDV(model);
    delete model;
}

void MainWindow::afficherStatistiquesRDV()
{
    QSqlQueryModel *model = RDV::statistiqueParJour();
    QString stats = "STATISTIQUES RDV PAR JOUR\n\n";

    if (model->rowCount() == 0) {
        stats += "Aucun rendez-vous enregistré.";
    } else {
        for (int i = 0; i < model->rowCount(); ++i) {
            QString jour = model->data(model->index(i, 0)).toString();
            int nb = model->data(model->index(i, 1)).toInt();
            stats += QString("• %1 : %2 RDV\n").arg(jour).arg(nb);
        }
    }

    delete model;
    QMessageBox::information(this, "Statistiques RDV", stats);
}

void MainWindow::chargerRDV()
{
    QSqlQueryModel *model = RDV::afficher();
    rafraichirTableRDV(model);
    delete model;
}

// دالة مساعدة عشان ما نكررش الكود
void MainWindow::rafraichirTableRDV(QSqlQueryModel *model)
{
    ui->tableWidget_8->setRowCount(0);

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget_8->insertRow(row);
        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(
                model->data(model->index(row, col)).toString()
                );
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget_8->setItem(row, col, item);
        }
    }
}

void MainWindow::selectionRDVChange()
{
    auto selected = ui->tableWidget_8->selectedItems();
    if (selected.isEmpty()) return;

    int row = selected.first()->row();

    ui->lineEdit_43->setText(ui->tableWidget_8->item(row, 0)->text());
    ui->lineEdit_48->setText(ui->tableWidget_8->item(row, 1)->text());

    // التاريخ
    QDate date = QDate::fromString(ui->tableWidget_8->item(row, 2)->text(), "dd/MM/yyyy");
    if (date.isValid()) ui->dateEdit->setDate(date);

    // الساعة
    QTime time = QTime::fromString(ui->tableWidget_8->item(row, 3)->text(), "HH:mm");
    if (time.isValid()) ui->timeEdit_2->setTime(time);

    ui->lineEdit_45->setText(ui->tableWidget_8->item(row, 4)->text());
}

// ==================== FONCTIONS fournisseur====================

void MainWindow::refreshFournisseurs()
{
    qDebug() << "Rafraîchissement des fournisseurs...";

    QSqlQueryModel *model = four.afficher();

    if (!model) {
        qDebug() << "Erreur: modèle null!";
        QMessageBox::warning(this, "Erreur", "Impossible de charger les fournisseurs (modèle null)");
        return;
    }

    if (model->lastError().isValid()) {
        QString error = model->lastError().text();
        qDebug() << "Erreur SQL:" << error;
        QMessageBox::critical(this, "Erreur SQL", "Erreur lors du chargement:\n" + error);
        delete model;
        return;
    }

    int rowCount = model->rowCount();
    int colCount = model->columnCount();

    qDebug() << "Nombre de lignes:" << rowCount << "Nombre de colonnes:" << colCount;

    ui->tableWidgetFournisseur->clear();
    ui->tableWidgetFournisseur->setRowCount(rowCount);
    ui->tableWidgetFournisseur->setColumnCount(colCount);

    // En-têtes
    ui->tableWidgetFournisseur->setHorizontalHeaderLabels({"ID", "Nom Société", "Adresse", "Email", "Téléphone", "Spécialité"});

    // Remplir le tableau
    for (int r = 0; r < rowCount; ++r) {
        for (int c = 0; c < colCount; ++c) {
            QString value = model->data(model->index(r, c)).toString();
            QTableWidgetItem *item = new QTableWidgetItem(value);
            ui->tableWidgetFournisseur->setItem(r, c, item);

            // Debug: afficher les premières lignes
            if (r < 3 && c == 0) {
                qDebug() << "Ligne" << r << "Col" << c << ":" << value;
            }
        }
    }

    delete model;

    // Redimensionner les colonnes
    ui->tableWidgetFournisseur->resizeColumnsToContents();

    // Message de statut
    if (rowCount > 0) {
        ui->statusbar->showMessage(QString("✅ %1 fournisseur(s) chargé(s)").arg(rowCount), 5000);
    } else {
        ui->statusbar->showMessage("📭 Aucun fournisseur dans la base de données", 5000);
        QMessageBox::information(this, "Information", "La table FOURNISSEUR est vide.\nAjoutez des fournisseurs d'abord.");
    }
}

void MainWindow::viderChampsfournisseur()
{
    ui->lineEditIdFournisseur->clear();
    ui->lineEditNomFournisseur->clear();
    ui->lineEditAdresseFournisseur->clear();
    ui->lineEditEmailFournisseur->clear();
    ui->lineEditTelFournisseur->clear();
    ui->comboSpecialite->setCurrentIndex(0);  // Remet sur la première option

    // Désélectionner le tableau
    ui->tableWidgetFournisseur->clearSelection();
}
void MainWindow::on_btnAjouterFournisseur_clicked()
{
    // Validation ID
    bool ok;
    qint64 id = ui->lineEditIdFournisseur->text().toLongLong(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "❌ Erreur", "ID invalide ! Entrez un nombre positif.");
        ui->lineEditIdFournisseur->setFocus();
        return;
    }

    // Vérifier si l'ID existe déjà
    QSqlQuery check;
    check.prepare("SELECT ID_FOURNISSEUR FROM FOURNISSEUR WHERE ID_FOURNISSEUR = :id");
    check.bindValue(":id", id);
    if (check.exec() && check.next()) {
        QMessageBox::critical(this, "❌ Erreur",
                              QString("L'ID <b>%1</b> existe déjà !<br>Choisissez un autre ID.").arg(id));
        ui->lineEditIdFournisseur->setFocus();
        return;
    }

    // Validation Nom
    QString nom = ui->lineEditNomFournisseur->text().trimmed();
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "❌ Erreur", "Le nom de société est obligatoire !");
        ui->lineEditNomFournisseur->setFocus();
        return;
    }

    // Création de l'objet Fournisseur
    Fournisseur f(id,
                  nom,
                  ui->lineEditAdresseFournisseur->text().trimmed(),
                  ui->lineEditEmailFournisseur->text().trimmed(),
                  ui->lineEditTelFournisseur->text().trimmed(),
                  ui->comboSpecialite->currentText());

    // Ajout dans la base
    if (f.ajouter()) {
        // LOG dans l'historique
        hist.logActionFournisseur(
            id,
            "AJOUT",
            "ADMIN",
            QString("Nouveau fournisseur créé → %1 | Spécialité: %2 | Email: %3")
                .arg(nom)
                .arg(ui->comboSpecialite->currentText())
                .arg(ui->lineEditEmailFournisseur->text())
            );

        QMessageBox::information(this, "✅ Succès",
                                 QString("Fournisseur <b>%1</b> ajouté avec succès !").arg(nom));

        refreshFournisseurs();
        viderChampsfournisseur();
    }
    else {
        QMessageBox::critical(this, "❌ Échec",
                              "Impossible d'ajouter le fournisseur !<br><br>"
                              "Erreur : " + QSqlDatabase::database().lastError().text());
    }
}
void MainWindow::on_btnModifierFournisseur_clicked()
{
    // Vérifier qu'une ligne est sélectionnée
    int row = ui->tableWidgetFournisseur->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "⚠️ Attention",
                             "Veuillez sélectionner un fournisseur dans le tableau !");
        return;
    }

    // Récupérer l'ID et l'ancien nom
    qint64 id = ui->tableWidgetFournisseur->item(row, 0)->text().toLongLong();
    QString ancienNom = ui->tableWidgetFournisseur->item(row, 1)->text();

    // Validation du nouveau nom
    QString nouveauNom = ui->lineEditNomFournisseur->text().trimmed();
    if (nouveauNom.isEmpty()) {
        QMessageBox::warning(this, "❌ Erreur", "Le nom de société ne peut pas être vide !");
        ui->lineEditNomFournisseur->setFocus();
        return;
    }

    // Confirmation
    auto reponse = QMessageBox::question(this, "⚠️ Confirmation",
                                         QString("Modifier le fournisseur <b>%1</b> ?<br><br>"
                                                 "Nouveau nom : <b>%2</b>").arg(ancienNom).arg(nouveauNom),
                                         QMessageBox::Yes | QMessageBox::No);

    if (reponse != QMessageBox::Yes) return;

    // Création de l'objet avec les nouvelles données
    Fournisseur f(id,
                  nouveauNom,
                  ui->lineEditAdresseFournisseur->text().trimmed(),
                  ui->lineEditEmailFournisseur->text().trimmed(),
                  ui->lineEditTelFournisseur->text().trimmed(),
                  ui->comboSpecialite->currentText());

    // Modification dans la base
    if (f.modifier()) {
        // LOG modification
        hist.logActionFournisseur(
            id,
            "MODIFICATION",
            "ADMIN",
            QString("Fournisseur modifié → ID: %1 | Ancien: %2 | Nouveau: %3")
                .arg(id).arg(ancienNom).arg(nouveauNom)
            );

        QMessageBox::information(this, "✅ Succès", "Fournisseur modifié avec succès !");
        refreshFournisseurs();
        viderChampsfournisseur();
    } else {
        QMessageBox::critical(this, "❌ Échec",
                              "Erreur lors de la modification !<br><br>" +
                                  QSqlDatabase::database().lastError().text());
    }
}
void MainWindow::on_btnSupprimerFournisseur_clicked()
{
    // Vérifier la sélection
    int row = ui->tableWidgetFournisseur->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "⚠️ Attention",
                             "Veuillez sélectionner un fournisseur à supprimer !");
        return;
    }

    qint64 id = ui->tableWidgetFournisseur->item(row, 0)->text().toLongLong();
    QString nom = ui->tableWidgetFournisseur->item(row, 1)->text();

    // Confirmation avec détails
    auto reponse = QMessageBox::question(this, "⚠️ CONFIRMATION DE SUPPRESSION",
                                         QString("<b style='color:red;'>⚠️ ATTENTION : Action irréversible !</b><br><br>"
                                                 "Voulez-vous vraiment supprimer :<br><br>"
                                                 "• <b>ID :</b> %1<br>"
                                                 "• <b>Société :</b> %2<br><br>"
                                                 "<i>Note : Si ce fournisseur est lié à des produits, "
                                                 "la suppression échouera.</i>").arg(id).arg(nom),
                                         QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes) {
        Fournisseur f;
        if (f.supprimer(id)) {
            // LOG suppression
            hist.logActionFournisseur(
                id,
                "SUPPRESSION",
                "ADMIN",
                QString("Fournisseur SUPPRIMÉ → ID: %1 | Société: %2").arg(id).arg(nom)
                );

            QMessageBox::information(this, "✅ Supprimé",
                                     "Fournisseur supprimé avec succès !");
            refreshFournisseurs();
            viderChampsfournisseur();
        } else {
            QMessageBox::critical(this, "❌ Échec de suppression",
                                  "Impossible de supprimer ce fournisseur !<br><br>"
                                  "<b>Causes possibles :</b><br>"
                                  "• Le fournisseur est lié à des produits existants<br>"
                                  "• Contrainte de clé étrangère (FOREIGN KEY)<br><br>"
                                  "<b>Solution :</b> Supprimez d'abord les produits liés à ce fournisseur.");
        }
    }
}
void MainWindow::on_btnAnnulerFournisseur_clicked()
{
    viderChampsfournisseur();

    QMessageBox::information(this, "✅ Annulé",
                             "Les champs ont été réinitialisés.<br>"
                             "Vous pouvez maintenant saisir un nouveau fournisseur.");
}
void MainWindow::on_btnRechercherFournisseur_clicked()
{
    QString critere = ui->lineEdit_15->text().trimmed();

    // Si vide, afficher tout
    if (critere.isEmpty()) {
        QMessageBox::information(this, "📋 Recherche",
                                 "Champ vide → Affichage de tous les fournisseurs");
        refreshFournisseurs();
        return;
    }

    // Recherche dans la base
    QSqlQueryModel *model = four.rechercher(critere);

    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "🔍 Aucun résultat",
                                 QString("Aucun fournisseur trouvé pour :<br><b>\"%1\"</b><br><br>"
                                         "💡 Essayez avec un autre terme.").arg(critere));
        if (model) delete model;
        return;
    }

    // Affichage des résultats
    ui->tableWidgetFournisseur->clear();
    ui->tableWidgetFournisseur->setRowCount(0);
    ui->tableWidgetFournisseur->setColumnCount(6);
    ui->tableWidgetFournisseur->setHorizontalHeaderLabels(
        {"ID", "Nom Société", "Adresse", "Email", "Téléphone", "Spécialité"});

    ui->tableWidgetFournisseur->setRowCount(model->rowCount());
    for (int r = 0; r < model->rowCount(); ++r) {
        for (int c = 0; c < 6; ++c) {
            QTableWidgetItem *item = new QTableWidgetItem(
                model->data(model->index(r, c)).toString());
            ui->tableWidgetFournisseur->setItem(r, c, item);
        }
    }

    delete model;
    ui->tableWidgetFournisseur->resizeColumnsToContents();

    // Message de succès
    QMessageBox::information(this, "✅ Recherche réussie",
                             QString("%1 résultat(s) trouvé(s) pour : <b>%2</b>")
                                 .arg(ui->tableWidgetFournisseur->rowCount())
                                 .arg(critere));
}
void MainWindow::on_btnTrierFournisseur_clicked()
{
    QString critere = ui->comboBox_8->currentText();

    if (critere.isEmpty() || critere == "Sélectionner") {
        QMessageBox::warning(this, "⚠️ Tri",
                             "Veuillez choisir un critère de tri !");
        return;
    }

    QString ordre = ui->radioButton_5->isChecked() ? "ASC" : "DESC";

    // Appel de la fonction de tri
    QSqlQueryModel *model = four.trier(critere, ordre);

    // Affichage des résultats triés
    ui->tableWidgetFournisseur->clear();
    ui->tableWidgetFournisseur->setRowCount(0);
    ui->tableWidgetFournisseur->setColumnCount(6);
    ui->tableWidgetFournisseur->setHorizontalHeaderLabels(
        {"ID", "Nom Société", "Adresse", "Email", "Téléphone", "Spécialité"});

    ui->tableWidgetFournisseur->setRowCount(model->rowCount());
    for (int r = 0; r < model->rowCount(); ++r) {
        for (int c = 0; c < 6; ++c) {
            ui->tableWidgetFournisseur->setItem(r, c,
                                                new QTableWidgetItem(model->data(model->index(r, c)).toString()));
        }
    }

    delete model;
    ui->tableWidgetFournisseur->resizeColumnsToContents();

    // Message dans la statusbar
    QString ordreTexte = (ordre == "ASC") ? "Croissant ↑" : "Décroissant ↓";
    ui->statusbar->showMessage(
        QString("✅ Tri appliqué : %1 - %2").arg(critere).arg(ordreTexte), 6000);
}
// ===================== STATISTIQUES FOURNISSEURS =====================
void MainWindow::on_btnStatFournisseur_clicked()
{
    // Récupère le nombre de fournisseurs par spécialité
    QMap<QString, int> stats;
    QSqlQuery query;
    query.exec("SELECT SPECIALITE, COUNT(*) FROM FOURNISSEUR GROUP BY SPECIALITE");

    while (query.next()) {
        QString specialite = query.value(0).toString().trimmed();
        if (specialite.isEmpty()) specialite = "Non renseignée";
        stats[specialite] = query.value(1).toInt();
    }

    if (stats.isEmpty()) {
        QMessageBox::information(this, "Statistiques", "Aucun fournisseur dans la base.");
        return;
    }

    // Création du graphique en camembert (Pie Chart)
    QPieSeries *series = new QPieSeries();
    QColor colors[] = {
        QColor("#FF6B6B"), QColor("#4ECDC4"), QColor("#45B7D1"),
        QColor("#96CEB4"), QColor("#FFEEAD"), QColor("#D4A5A5"),
        QColor("#9B59B6"), QColor("#3498DB"), QColor("#E74C3C")
    };
    int colorIndex = 0;

    for (auto it = stats.constBegin(); it != stats.constEnd(); ++it) {
        QPieSlice *slice = series->append(it.key() + QString(" (%1)").arg(it.value()), it.value());
        slice->setLabelVisible(true);
        slice->setBrush(colors[colorIndex % 9]);
        colorIndex++;
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("<b>Répartition des fournisseurs par spécialité</b>");
    chart->setTitleFont(QFont("Arial", 16));
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setBackgroundRoundness(20);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(900, 600);
    chartView->setWindowTitle("Statistiques Fournisseurs");
    chartView->show();
}

// ===================== EXPORT PDF FOURNISSEURS =====================
void MainWindow::on_btnExporterFournisseur_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Exporter en PDF"),
                                                    QString("liste_fournisseurs_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                    tr("PDF Files (*.pdf)"));

    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    // Marges
    QPageLayout layout = writer.pageLayout();
    layout.setMargins(QMarginsF(20, 20, 20, 20));
    writer.setPageLayout(layout);

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);

    // === EN-TÊTE ===
    QFont titleFont = painter.font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(0, 100, writer.width(), 100, Qt::AlignCenter, "LISTE DES FOURNISSEURS");

    QFont subtitleFont = painter.font();
    subtitleFont.setPointSize(10);
    subtitleFont.setBold(false);
    painter.setFont(subtitleFont);
    painter.drawText(0, 180, writer.width(), 50, Qt::AlignCenter,
                     "Généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm"));

    painter.drawLine(50, 250, writer.width() - 50, 250);

    // === TABLEAU ===
    QTableWidget *table = ui->tableWidgetFournisseur;
    int cols = table->columnCount();
    int rows = table->rowCount();

    int startY = 300;
    int rowHeight = 40;
    int headerHeight = 50;

    // Calcul des largeurs de colonnes
    QVector<int> colWidths;
    int totalWidth = writer.width() - 100;

    // Répartition proportionnelle
    colWidths << totalWidth * 0.10;  // ID
    colWidths << totalWidth * 0.25;  // Nom Société
    colWidths << totalWidth * 0.25;  // Adresse
    colWidths << totalWidth * 0.15;  // Email
    colWidths << totalWidth * 0.10;  // Téléphone
    colWidths << totalWidth * 0.15;  // Spécialité

    // === EN-TÊTES DE COLONNES ===
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(QColor(200, 200, 200))); // Gris clair pour l'en-tête

    int xPos = 50;
    for (int col = 0; col < cols; ++col) {
        QString header = table->horizontalHeaderItem(col) ?
                             table->horizontalHeaderItem(col)->text() :
                             QString("Col%1").arg(col);

        painter.drawRect(xPos, startY, colWidths[col], headerHeight);

        QFont headerFont = painter.font();
        headerFont.setPointSize(8);
        headerFont.setBold(true);
        painter.setFont(headerFont);
        painter.drawText(xPos + 5, startY + 5, colWidths[col] - 10, headerHeight - 10,
                         Qt::AlignCenter | Qt::TextWordWrap, header);

        xPos += colWidths[col];
    }

    // === DONNÉES ===
    int currentY = startY + headerHeight;
    QFont dataFont = painter.font();
    dataFont.setPointSize(7);
    dataFont.setBold(false);
    painter.setFont(dataFont);

    for (int row = 0; row < rows; ++row) {
        // Nouvelle page si nécessaire
        if (currentY > writer.height() - 100) {
            writer.newPage();
            currentY = 100;

            // Redessiner les en-têtes
            xPos = 50;
            painter.setBrush(QBrush(QColor(200, 200, 200)));
            for (int col = 0; col < cols; ++col) {
                QString header = table->horizontalHeaderItem(col) ?
                                     table->horizontalHeaderItem(col)->text() :
                                     QString("Col%1").arg(col);
                painter.drawRect(xPos, currentY, colWidths[col], headerHeight);

                QFont headerFont = painter.font();
                headerFont.setPointSize(8);
                headerFont.setBold(true);
                painter.setFont(headerFont);
                painter.drawText(xPos + 5, currentY + 5, colWidths[col] - 10, headerHeight - 10,
                                 Qt::AlignCenter | Qt::TextWordWrap, header);
                xPos += colWidths[col];
            }
            currentY += headerHeight;
        }

        // Alternance de couleurs pour les lignes
        if (row % 2 == 0) {
            painter.setBrush(QBrush(QColor(240, 240, 240))); // Gris très clair
        } else {
            painter.setBrush(QBrush(Qt::white));
        }

        // Dessiner les données de la ligne
        xPos = 50;
        for (int col = 0; col < cols; ++col) {
            QString text = table->item(row, col) ? table->item(row, col)->text() : "";

            painter.drawRect(xPos, currentY, colWidths[col], rowHeight);
            painter.drawText(xPos + 5, currentY + 5, colWidths[col] - 10, rowHeight - 10,
                             Qt::AlignLeft | Qt::TextWordWrap, text);

            xPos += colWidths[col];
        }
        currentY += rowHeight;
    }

    // === PIED DE PAGE ===
    painter.setFont(subtitleFont);
    painter.drawText(0, writer.height() - 50, writer.width(), 30, Qt::AlignCenter,
                     QString("Page 1 - %1 fournisseur(s)").arg(rows));

    painter.end();

    QMessageBox::information(this, "Export PDF Réussi",
                             "Liste des fournisseurs exportée avec succès !\n\n"
                             "Fichier : " + fileName);
}

// ===================== CLIC SUR TABLEAU FOURNISSEURS =====================
void MainWindow::on_tableWidgetFournisseur_cellClicked(int row, int column)
{
    Q_UNUSED(column);

    // Vérifier que la ligne est valide
    if (row < 0 || row >= ui->tableWidgetFournisseur->rowCount()) {
        return;
    }

    // Remplir les champs du formulaire avec les données de la ligne sélectionnée
    QTableWidgetItem *itemID = ui->tableWidgetFournisseur->item(row, 0);
    QTableWidgetItem *itemNom = ui->tableWidgetFournisseur->item(row, 1);
    QTableWidgetItem *itemAdresse = ui->tableWidgetFournisseur->item(row, 2);
    QTableWidgetItem *itemEmail = ui->tableWidgetFournisseur->item(row, 3);
    QTableWidgetItem *itemTel = ui->tableWidgetFournisseur->item(row, 4);
    QTableWidgetItem *itemSpec = ui->tableWidgetFournisseur->item(row, 5);

    if (itemID) ui->lineEditIdFournisseur->setText(itemID->text());
    if (itemNom) ui->lineEditNomFournisseur->setText(itemNom->text());
    if (itemAdresse) ui->lineEditAdresseFournisseur->setText(itemAdresse->text());
    if (itemEmail) ui->lineEditEmailFournisseur->setText(itemEmail->text());
    if (itemTel) ui->lineEditTelFournisseur->setText(itemTel->text());
    if (itemSpec) ui->comboSpecialite->setCurrentText(itemSpec->text());
}
// ===================== HISTORIQUE - VERSION FINALE =====================

void MainWindow::on_btnHistoriqueFournisseurs_clicked()
{
    // ⚠️ CHANGEZ "page_5" par le BON nom de votre page historique
    ui->stackedWidget->setCurrentWidget(ui->page_3);
    // OU utilisez l'index si vous connaissez le numéro exact :
    // ui->stackedWidget->setCurrentIndex(7);  // par exemple

    // Vérifier que les widgets existent avant de les utiliser
    if (!ui->dateEditHistFrom || !ui->dateEditHistTo) {
        QMessageBox::critical(this, "Erreur",
                              "Les widgets de date sont introuvables !\n"
                              "Vérifiez les noms dans Qt Designer.");
        return;
    }

    if (!ui->lineEditHistRecherche || !ui->comboHistAction) {
        QMessageBox::critical(this, "Erreur",
                              "Les widgets de filtrage sont introuvables !\n"
                              "Vérifiez les noms dans Qt Designer.");
        return;
    }

    if (!ui->tableWidgetHistorique) {
        QMessageBox::critical(this, "Erreur",
                              "Le tableau d'historique est introuvable !\n"
                              "Vérifiez le nom dans Qt Designer.");
        return;
    }

    // Initialiser les dates (30 derniers jours)
    ui->dateEditHistFrom->setDate(QDate::currentDate().addDays(-30));
    ui->dateEditHistTo->setDate(QDate::currentDate());

    // Réinitialiser les filtres
    ui->lineEditHistRecherche->clear();
    ui->comboHistAction->setCurrentIndex(0);

    // Afficher tout l'historique
    refreshHistorique();

    QMessageBox::information(this, "📊 Historique",
                             "Affichage des actions des 30 derniers jours.<br><br>"
                             "💡 Utilisez les filtres pour affiner votre recherche.");
}

void MainWindow::on_btnHistFiltrer_clicked()
{
    QString term = ui->lineEditHistRecherche->text().trimmed();
    QString action = ui->comboHistAction->currentText();
    QString from = ui->dateEditHistFrom->date().toString("yyyy-MM-dd");
    QString to = ui->dateEditHistTo->date().toString("yyyy-MM-dd");

    // Validation des dates
    if (ui->dateEditHistFrom->date() > ui->dateEditHistTo->date()) {
        QMessageBox::warning(this, "⚠️ Dates invalides",
                             "La date de début doit être antérieure à la date de fin !");
        return;
    }

    QSqlQueryModel *model = hist.filtrer(term, action, from, to);

    if (model) {
        int rowCount = model->rowCount();
        populateTableHistorique(model);

        if (rowCount == 0) {
            QMessageBox::information(this, "🔍 Aucun résultat",
                                     "Aucun enregistrement ne correspond aux critères.<br><br>"
                                     "💡 Essayez d'élargir la période ou de modifier les filtres.");
        } else {
            ui->statusbar->showMessage(
                QString("✅ %1 enregistrement(s) trouvé(s)").arg(rowCount), 5000);
        }

        delete model;
    } else {
        QMessageBox::critical(this, "❌ Erreur",
                              "Erreur lors du filtrage de l'historique !");
    }
}

void MainWindow::on_btnHistRefresh_clicked()
{
    // Réinitialiser tous les filtres
    ui->lineEditHistRecherche->clear();
    ui->comboHistAction->setCurrentIndex(0);
    ui->dateEditHistFrom->setDate(QDate::currentDate().addDays(-30));
    ui->dateEditHistTo->setDate(QDate::currentDate());

    // Recharger tout
    refreshHistorique();

    QMessageBox::information(this, "🔄 Actualisation",
                             "Tous les filtres ont été réinitialisés.<br>"
                             "Affichage complet de l'historique des 30 derniers jours.");
}
void MainWindow::on_btnHistExporterPDF_clicked()
{
    QString term = ui->lineEditHistRecherche->text().trimmed();
    QString action = ui->comboHistAction->currentText();
    QString from = ui->dateEditHistFrom->date().toString("yyyy-MM-dd");
    QString to = ui->dateEditHistTo->date().toString("yyyy-MM-dd");

    QSqlQueryModel *model;

    // Vérifier si des filtres sont actifs
    if (term.isEmpty() && (action.isEmpty() || action == "Tous") &&
        from == QDate::currentDate().addDays(-30).toString("yyyy-MM-dd") &&
        to == QDate::currentDate().toString("yyyy-MM-dd")) {
        model = hist.afficherAll();
    } else {
        model = hist.filtrer(term, action, from, to);
    }

    if (!model || model->rowCount() == 0) {
        QMessageBox::warning(this, "Export PDF", "Aucune donnée à exporter.");
        if (model) delete model;
        return;
    }

    // Générer le titre du PDF
    QString title = "Historique des Fournisseurs";

    if (!term.isEmpty()) {
        title += QString(" - Recherche: \"%1\"").arg(term);
    }
    if (!action.isEmpty() && action != "Tous") {
        title += QString(" - Action: %1").arg(action);
    }

    // Exporter en PDF
    QString file = hist.exportToPdf(model, title, "historique_fournisseurs");
    delete model;

    if (!file.isEmpty()) {
        QMessageBox::information(this, "✅ Export PDF Réussi",
                                 "L'historique a été exporté avec succès !\n\n"
                                 "Fichier : " + file);
    } else {
        QMessageBox::warning(this, "Erreur d'Export", "Échec de l'export PDF.");
    }
}

void MainWindow::on_comboHistAction_currentIndexChanged(int index)
{
    if (index == 0) {  // "Tous" sélectionné
        ui->statusbar->showMessage(
            "💡 Cliquez sur « Rechercher » pour afficher tous les enregistrements", 3000);
    } else {
        QString action = ui->comboHistAction->currentText();
        ui->statusbar->showMessage(
            QString("Action sélectionnée : %1 → Cliquez sur « Rechercher »").arg(action), 4000);
    }
}

void MainWindow::refreshHistorique()
{
    QSqlQueryModel *model = hist.afficherAll();
    populateTableHistorique(model);
    delete model;
}
void MainWindow::on_pushButton_4_clicked()
{
    refreshHistorique();  // bouton résiduel
}

void MainWindow::on_pushButton_3_clicked()
{
    on_btnHistFiltrer_clicked();
}

void MainWindow::on_pushButton_5_clicked()
{
    on_btnHistExporterPDF_clicked();
}


void MainWindow::populateTableHistorique(QSqlQueryModel *model)
{
    ui->tableWidgetHistorique->clear();
    ui->tableWidgetHistorique->setRowCount(0);

    if (!model || model->rowCount() == 0) {
        ui->tableWidgetHistorique->setColumnCount(1);
        ui->tableWidgetHistorique->setHorizontalHeaderLabels({"Message"});
        ui->tableWidgetHistorique->setRowCount(1);
        ui->tableWidgetHistorique->setItem(0, 0, new QTableWidgetItem("Aucun historique"));
        return;
    }

    // === 6 colonnes visibles seulement (les plus importantes) ===
    ui->tableWidgetHistorique->setColumnCount(6);
    QStringList headers = {
        "ID Fournisseur",
        "Nom Société",
        "Action",
        "Date Action",
        "Utilisateur",
        "Détails"
    };
    ui->tableWidgetHistorique->setHorizontalHeaderLabels(headers);

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidgetHistorique->insertRow(row);

        // Récupération des données selon l'ordre EXACT de ta requête Oracle
        QString idFournisseur = model->data(model->index(row, 1)).toString();  // ID_FOURNISSEUR
        QString nomSociete    = model->data(model->index(row, 2)).toString();  // NOM_SOCIETE
        QString action        = model->data(model->index(row, 9)).toString();  // ACTION
        QString dateAction    = model->data(model->index(row, 10)).toString(); // DATE_ACTION (déjà formatée)
        QString utilisateur   = model->data(model->index(row, 11)).toString(); // UTILISATEUR
        QString details       = model->data(model->index(row, 12)).toString(); // DETAILS

        // === Remplissage ===
        ui->tableWidgetHistorique->setItem(row, 0, new QTableWidgetItem(idFournisseur));
        ui->tableWidgetHistorique->setItem(row, 1, new QTableWidgetItem(nomSociete.isEmpty() ? "—" : nomSociete));

        // === Action avec couleur ===
        QTableWidgetItem *itemAction = new QTableWidgetItem(action.trimmed());
        itemAction->setTextAlignment(Qt::AlignCenter);
        itemAction->setFont(QFont("Segoe UI", 10, QFont::Bold));

        if (action.contains("AJOUT", Qt::CaseInsensitive)) {
            itemAction->setBackground(QColor(200, 255, 200));
            itemAction->setForeground(QColor(0, 100, 0));
        } else if (action.contains("MODIFICATION", Qt::CaseInsensitive)) {
            itemAction->setBackground(QColor(173, 216, 255));
            itemAction->setForeground(QColor(0, 0, 139));
        } else if (action.contains("SUPPRESSION", Qt::CaseInsensitive)) {
            itemAction->setBackground(QColor(255, 182, 193));
            itemAction->setForeground(QColor(139, 0, 0));
        }
        ui->tableWidgetHistorique->setItem(row, 2, itemAction);

        // Date déjà bien formatée par TO_CHAR
        ui->tableWidgetHistorique->setItem(row, 3, new QTableWidgetItem(dateAction));

        // Utilisateur
        ui->tableWidgetHistorique->setItem(row, 4, new QTableWidgetItem(utilisateur.isEmpty() ? "ADMIN" : utilisateur));

        // Détails (avec tooltip complet)
        QTableWidgetItem *itemDetails = new QTableWidgetItem(details.isEmpty() ? "Aucun détail" : details);
        itemDetails->setToolTip(details);
        itemDetails->setForeground(QColor(70, 70, 70));
        ui->tableWidgetHistorique->setItem(row, 5, itemDetails);
    }

    ui->tableWidgetHistorique->resizeColumnsToContents();
    ui->tableWidgetHistorique->horizontalHeader()->setStretchLastSection(true);
}
// ===================================================================
// SLOT : ComboBox "Tous" → Filtre par type d'action (Tous / AJOUT / MODIFICATION / SUPPRESSION)
// ===================================================================
// ===================================================================
// SLOT : Filtre par type d'action → comboBox nommé "comboHistAction"
// Items : Tous | AJOUT | MODIFICATION | SUPPRESSION
// ===================================================================

void MainWindow::testHistoriqueForce()
{
    qDebug() << "TEST FORCÉ : insertion directe dans l'historique...";

    QSqlQuery q;
    bool ok = q.exec("INSERT INTO HISTORIQUE_FOURNISSEUR (ID_FOURNISSEUR, ACTION, DATE_ACTION, UTILISATEUR, DETAILS) "
                     "VALUES (999999, 'TEST_FORCÉ', SYSDATE, 'ADMIN', 'Ceci est un test visible')");

    if (ok) {
        qDebug() << "TEST RÉUSSI → ligne insérée ! Rafraîchis l'historique maintenant.";
    } else {
        qDebug() << "ÉCHEC TEST →" << q.lastError().text();
    }
}


// ==================== FONCTIONS produit ====================

void MainWindow::on_btnAjouterProduit_clicked()
{


    bool ok;
    int ref = ui->refProduit->text().toInt(&ok);
    if (!ok || ref <= 0) {
        QMessageBox::warning(this, "Erreur", "Référence invalide !");
        return;
    }

    QString cat = ui->categorieProduit->currentText();
    QString marque = ui->marqueProduit->text().trimmed();
    if (marque.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Marque obligatoire !");
        return;
    }

    double prix = ui->prixProduit->text().toDouble(&ok);
    if (!ok || prix <= 0) {
        QMessageBox::warning(this, "Erreur", "Prix invalide !");
        return;
    }

    int qte = ui->quantiteProduit->text().toInt(&ok);
    if (!ok || qte < 0) {
        QMessageBox::warning(this, "Erreur", "Quantité invalide !");
        return;
    }

    QDate dateAjout = QDate::fromString(ui->date_p->text(), "dd/MM/yyyy");
    if (!dateAjout.isValid()) {
        QMessageBox::warning(this, "Erreur", "Format de date incorrect (jj/mm/aaaa) !");
        return;
    }

    Produit p(ref, cat, marque, prix, qte, dateAjout);

    // VÉRIFICATION SI LA RÉFÉRENCE EXISTE DÉJÀ
    QSqlQuery check;
    check.prepare("SELECT REF_P FROM PRODUIT WHERE REF_P = ?");
    check.addBindValue(ref);
    if (check.exec() && check.next()) {
        QMessageBox::critical(this, "Erreur", "Cette référence existe déjà !\nChoisis une autre référence.");
        return;
    }

    if (p.ajouter()) {
        QMessageBox::information(this, "Succès", QString("Produit %1 ajouté avec succès !").arg(ref));
        // Remettre la date du jour pour le prochain ajout
        ui->date_p->setText(QDate::currentDate().toString("dd/MM/yyyy"));
        // Vider les champs si tu veux
        ui->refProduit->clear();
        ui->marqueProduit->clear();
        ui->prixProduit->clear();
        ui->quantiteProduit->clear();
        updateIntelligente();
    } else {
        QMessageBox::critical(this, "Échec ajout", "Erreur base de données :\n" );
    }
}
//mod
void MainWindow::on_btnModifierProduit_clicked()
{

    int ref = ui->refProduit->text().toInt();
    QString cat = ui->categorieProduit->currentText();
    QString marque = ui->marqueProduit->text();
    double prix = ui->prixProduit->text().toDouble();
    int qte = ui->quantiteProduit->text().toInt();
    QDate dateAjout = QDate::fromString(ui->date_p->text(), "dd/MM/yyyy");
    Produit p(ref, cat, marque, prix, qte,dateAjout);
    if (p.modifier()) {
        QMessageBox::information(this, "Succès", "Produit modifié !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec modification");
        return;
    }
    updateIntelligente();
}
//annuler
void MainWindow::on_btnAnnulerProduit_clicked()
{

    if (QMessageBox::question(this, "Confirmer", "Supprimer TOUS les produits ?") == QMessageBox::Yes) {
        QSqlQuery q; q.exec("DELETE FROM PRODUIT");
        afficherProduits();
        updateIntelligente();
        QMessageBox::information(this, "Succès", "Tous les produits supprimés.");
    }
}
//supp
void MainWindow::on_btnSupprimerProduit_clicked()
{


    // Récupérer la ligne actuellement sélectionnée dans le tableau
    int row = ui->tableWidget_2->currentRow();

    if (row == -1) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un produit dans le tableau !");
        return;
    }

    // Récupérer la référence depuis la colonne 0 de la ligne sélectionnée
    QTableWidgetItem* item = ui->tableWidget_2->item(row, 0);
    if (!item) {
        QMessageBox::warning(this, "Erreur", "Référence introuvable !");
        return;
    }

    bool ok;
    int ref = item->text().toInt(&ok);
    if (!ok || ref <= 0) {
        QMessageBox::warning(this, "Erreur", "Référence invalide !");
        return;
    }

    // Confirmation avec la marque pour être sûr
    QString marque = ui->tableWidget_2->item(row, 1)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmer suppression",
                                  QString("Supprimer le produit :<br><b>%1 - %2</b> ?").arg(ref).arg(marque),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        Produit p;
        if (p.annuler(ref)) {
            QMessageBox::information(this, "Succès", QString("Produit %1 supprimé !").arg(ref));
            afficherProduits();      // Rafraîchit le tableau
            updateIntelligente();    // Recalcule remise + anomalies
        } else {
            QMessageBox::critical(this, "Erreur",
                                  "Échec de la suppression !\n" + QSqlDatabase::database().lastError().text());
        }
    }
}

void MainWindow::on_btnRechercherProduit_clicked()
{

    QString critere = ui->lineEdit_21->text();
    QSqlQueryModel *model = prod.rechercher(critere);
    ui->tableWidget_2->setRowCount(model->rowCount());
    ui->tableWidget_2->setColumnCount(5);
    for (int i = 0; i < model->rowCount(); ++i) {
        for (int j = 0; j < 5; ++j) {
            ui->tableWidget_2->setItem(i, j, new QTableWidgetItem(model->data(model->index(i, j)).toString()));
        }
    }
    delete model;
}
//trie
void MainWindow::on_btnTrierProduit_clicked()
{


    QString critere = ui->comboTriProduit->currentText();
    QString ordre = ui->radioButton_asc->isChecked() ? "ASC" : "DESC";

    // Correspondance claire et directe
    QString champSQL;
    if (critere == "Prix") {
        champSQL = "PRIX";
    }
    else if (critere == "Quantité en stock") {
        champSQL = "QUANTITE";
    }
    else if (critere == "Date d'ajout") {
        champSQL = "DATE_P";
    }
    else {
        QMessageBox::warning(this, "Tri", "Veuillez choisir un critère valide !");
        return;
    }

    // Appel de la fonction trier() de la classe Produit
    QSqlQueryModel *model = prod.trier(champSQL, ordre);

    // On remplit le tableWidget avec les 6 colonnes (comme dans afficherProduits)
    ui->tableWidget_2->setRowCount(model->rowCount());
    ui->tableWidget_2->setColumnCount(6);

    for (int i = 0; i < model->rowCount(); ++i) {
        ui->tableWidget_2->setItem(i, 0, new QTableWidgetItem(model->data(model->index(i, 0)).toString())); // Ref
        ui->tableWidget_2->setItem(i, 1, new QTableWidgetItem(model->data(model->index(i, 1)).toString())); // Marque
        ui->tableWidget_2->setItem(i, 2, new QTableWidgetItem(model->data(model->index(i, 2)).toString())); // Catégorie
        ui->tableWidget_2->setItem(i, 3, new QTableWidgetItem(model->data(model->index(i, 3)).toString() + " DT")); // Prix + DT
        ui->tableWidget_2->setItem(i, 4, new QTableWidgetItem(model->data(model->index(i, 4)).toString())); // Quantité
        // Date formatée en jj/mm/aaaa
        QDate date = model->data(model->index(i, 5)).toDate();
        ui->tableWidget_2->setItem(i, 5, new QTableWidgetItem(date.toString("dd/MM/yyyy")));
    }

    delete model;
    ui->tableWidget_2->resizeColumnsToContents();
}


void MainWindow::on_btnExporterProduit_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Exporter en PDF"),
                                                    QString("liste_produits_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")),
                                                    tr("PDF Files (*.pdf)"));

    if (fileName.isEmpty()) return;

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    // Marges
    QPageLayout layout = writer.pageLayout();
    layout.setMargins(QMarginsF(20, 20, 20, 20));
    writer.setPageLayout(layout);

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);

    // === EN-TÊTE ===
    QFont titleFont = painter.font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    painter.setFont(titleFont);
    painter.drawText(0, 100, writer.width(), 100, Qt::AlignCenter, "LISTE DES PRODUITS");

    QFont subtitleFont = painter.font();
    subtitleFont.setPointSize(10);
    subtitleFont.setBold(false);
    painter.setFont(subtitleFont);
    painter.drawText(0, 180, writer.width(), 50, Qt::AlignCenter,
                     "Généré le " + QDateTime::currentDateTime().toString("dd/MM/yyyy à HH:mm"));

    painter.drawLine(50, 250, writer.width() - 50, 250);

    // === TABLEAU ===
    QTableWidget *table = ui->tableWidget_2;
    int cols = table->columnCount();
    int rows = table->rowCount();

    int startY = 300;
    int rowHeight = 40;
    int headerHeight = 50;

    // Calcul des largeurs de colonnes
    QVector<int> colWidths;
    int totalWidth = writer.width() - 100;

    // Répartition proportionnelle
    colWidths << totalWidth * 0.10;  // ref
    colWidths << totalWidth * 0.25;  // cat
    colWidths << totalWidth * 0.25;  // marque
    colWidths << totalWidth * 0.15;  // prix
    colWidths << totalWidth * 0.10;  // qte
    colWidths << totalWidth * 0.15;  // date

    // === EN-TÊTES DE COLONNES ===
    painter.setPen(QPen(Qt::black));
    painter.setBrush(QBrush(QColor(200, 200, 200))); // Gris clair pour l'en-tête

    int xPos = 50;
    for (int col = 0; col < cols; ++col) {
        QString header = table->horizontalHeaderItem(col) ?
                             table->horizontalHeaderItem(col)->text() :
                             QString("Col%1").arg(col);

        painter.drawRect(xPos, startY, colWidths[col], headerHeight);

        QFont headerFont = painter.font();
        headerFont.setPointSize(8);
        headerFont.setBold(true);
        painter.setFont(headerFont);
        painter.drawText(xPos + 5, startY + 5, colWidths[col] - 10, headerHeight - 10,
                         Qt::AlignCenter | Qt::TextWordWrap, header);

        xPos += colWidths[col];
    }

    // === DONNÉES ===
    int currentY = startY + headerHeight;
    QFont dataFont = painter.font();
    dataFont.setPointSize(7);
    dataFont.setBold(false);
    painter.setFont(dataFont);

    for (int row = 0; row < rows; ++row) {
        // Nouvelle page si nécessaire
        if (currentY > writer.height() - 100) {
            writer.newPage();
            currentY = 100;

            // Redessiner les en-têtes
            xPos = 50;
            painter.setBrush(QBrush(QColor(200, 200, 200)));
            for (int col = 0; col < cols; ++col) {
                QString header = table->horizontalHeaderItem(col) ?
                                     table->horizontalHeaderItem(col)->text() :
                                     QString("Col%1").arg(col);
                painter.drawRect(xPos, currentY, colWidths[col], headerHeight);

                QFont headerFont = painter.font();
                headerFont.setPointSize(8);
                headerFont.setBold(true);
                painter.setFont(headerFont);
                painter.drawText(xPos + 5, currentY + 5, colWidths[col] - 10, headerHeight - 10,
                                 Qt::AlignCenter | Qt::TextWordWrap, header);
                xPos += colWidths[col];
            }
            currentY += headerHeight;
        }

        // Alternance de couleurs pour les lignes
        if (row % 2 == 0) {
            painter.setBrush(QBrush(QColor(240, 240, 240))); // Gris très clair
        } else {
            painter.setBrush(QBrush(Qt::white));
        }

        // Dessiner les données de la ligne
        xPos = 50;
        for (int col = 0; col < cols; ++col) {
            QString text = table->item(row, col) ? table->item(row, col)->text() : "";

            painter.drawRect(xPos, currentY, colWidths[col], rowHeight);
            painter.drawText(xPos + 5, currentY + 5, colWidths[col] - 10, rowHeight - 10,
                             Qt::AlignLeft | Qt::TextWordWrap, text);

            xPos += colWidths[col];
        }
        currentY += rowHeight;
    }

    // === PIED DE PAGE ===
    painter.setFont(subtitleFont);
    painter.drawText(0, writer.height() - 50, writer.width(), 30, Qt::AlignCenter,
                     QString("Page 1 - %1 produits(s)").arg(rows));

    painter.end();

    QMessageBox::information(this, "Export PDF Réussi",
                             "Liste des produits exportée avec succès !\n\n"
                             "Fichier : " + fileName);
}
void MainWindow::on_pushButton_stat_clicked()
{

    afficherStatistiques();
}

void MainWindow::on_pushButton_anomalies_clicked()
{
    detecterAnomalies();
}
void MainWindow::on_pushButton_remiseta_clicked()
{
    appliquerRemiseAuto();
}

void MainWindow::appliquerRemiseAuto()
{
    ui->tableWidget_remise->clear();
    ui->tableWidget_remise->setRowCount(0);
    ui->tableWidget_remise->setColumnCount(4);
    ui->tableWidget_remise->setHorizontalHeaderLabels({"Référence", "Marque", "Ancien prix", "Nouveau prix"});

    QSqlQuery query;

    // REQUÊTE COMPATIBLE ORACLE / MySQL / PostgreSQL
    QString sql = R"(
        SELECT REF_P, MARQUE, PRIX, QUANTITE, DATE_P
        FROM PRODUIT
        WHERE QUANTITE > 50
          AND DATE_P <= CURRENT_DATE - INTERVAL '60' DAY
    )";

    if (!query.exec(sql)) {
        qDebug() << "ERREUR SQL REMISE:" << query.lastError().text();
        ui->tableWidget_remise->insertRow(0);
        ui->tableWidget_remise->setItem(0, 0, new QTableWidgetItem("Erreur SQL"));
        ui->tableWidget_remise->setItem(0, 1, new QTableWidgetItem(query.lastError().text()));
        return;
    }

    int count = 0;
    int row = 0;

    while (query.next()) {
        int ref = query.value(0).toInt();
        QString marque = query.value(1).toString();
        double ancienPrix = query.value(2).toDouble();
        double nouveauPrix = ancienPrix * 0.70;

        // Mise à jour du prix
        QSqlQuery update;
        update.prepare("UPDATE PRODUIT SET PRIX = :nouveau WHERE REF_P = :ref");
        update.bindValue(":nouveau", nouveauPrix);
        update.bindValue(":ref", ref);
        update.exec();

        // Affichage dans le tableau
        ui->tableWidget_remise->insertRow(row);
        ui->tableWidget_remise->setItem(row, 0, new QTableWidgetItem(QString::number(ref)));
        ui->tableWidget_remise->setItem(row, 1, new QTableWidgetItem(marque));
        ui->tableWidget_remise->setItem(row, 2, new QTableWidgetItem(QString::number(ancienPrix, 'f', 2)));
        ui->tableWidget_remise->setItem(row, 3, new QTableWidgetItem(QString::number(nouveauPrix, 'f', 2) + " DT"));

        // Vert gras
        for (int c = 0; c < 4; ++c) {
            auto item = ui->tableWidget_remise->item(row, c);
            item->setForeground(QBrush(QColor(0, 140, 0)));
            item->setFont(QFont("Arial", 10, QFont::Bold));
        }
        row++;
        count++;
    }

    if (count == 0) {
        ui->tableWidget_remise->insertRow(0);
        ui->tableWidget_remise->setItem(0, 0, new QTableWidgetItem("Aucun produit éligible"));
        ui->tableWidget_remise->setItem(0, 1, new QTableWidgetItem("Stock > 50 + date > 60 jours"));
    } else {
        ui->statusbar->showMessage(QString("Remise -30% appliquée sur %1 produit(s) !").arg(count), 15000);
    }

    ui->tableWidget_remise->resizeColumnsToContents();
    afficherProduits();
}

void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    ui->stackedWidget->setCurrentIndex(3);
    Q_UNUSED(column);
    ui->refProduit->setText(ui->tableWidget_2->item(row, 0)->text());
    ui->categorieProduit->setCurrentText(ui->tableWidget_2->item(row, 2)->text());   // Catégorie
    ui->marqueProduit->setText(ui->tableWidget_2->item(row, 1)->text());        // Marque
    ui->prixProduit->setText(ui->tableWidget_2->item(row, 3)->text().remove(" DT"));
    ui->quantiteProduit->setText(ui->tableWidget_2->item(row, 4)->text());
    ui->date_p->setText(ui->tableWidget_2->item(row, 5)->text());
}

// ===================== AFFICHAGE PRODUITS =====================
void MainWindow::afficherProduits()
{
    // Pas de changement de page ici !
    ui->tableWidget_2->setRowCount(0);
    ui->tableWidget_2->setColumnCount(6);
    QStringList headers = {"Ref", "Marque", "Catégorie", "Prix", "Quantité en stock", "Date d'ajout"};
    ui->tableWidget_2->setHorizontalHeaderLabels(headers);

    QSqlQuery q("SELECT REF_P, MARQUE, CATEGORIE, PRIX, QUANTITE, DATE_P FROM PRODUIT ORDER BY REF_P");
    int row = 0;
    while (q.next()) {
        ui->tableWidget_2->insertRow(row);
        ui->tableWidget_2->setItem(row, 0, new QTableWidgetItem(q.value(0).toString()));
        ui->tableWidget_2->setItem(row, 1, new QTableWidgetItem(q.value(1).toString()));
        ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(q.value(2).toString()));
        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(q.value(3).toString() + " DT"));
        ui->tableWidget_2->setItem(row, 4, new QTableWidgetItem(q.value(4).toString()));
        ui->tableWidget_2->setItem(row, 5, new QTableWidgetItem(q.value(5).toDate().toString("dd/MM/yyyy")));
        row++;
    }
    ui->tableWidget_2->resizeColumnsToContents();

    qDebug() << "✅ Produits affichés:" << row << "lignes";
}
void MainWindow::updateIntelligente()
{
    appliquerRemiseAuto();
    detecterAnomalies();
}
// ===================== REMISE & ANOMALIES =====================


void MainWindow::detecterAnomalies()
{
    ui->tableWidget_anomalies->setRowCount(0);  // Vider le tableau
    int row = 0;

    QSqlQuery q;
    q.exec("SELECT REF_P, MARQUE, QUANTITE, DATE_P FROM PRODUIT");

    while (q.next()) {
        int ref = q.value("REF_P").toInt();
        QString marque = q.value("MARQUE").toString();
        int quantite = q.value("QUANTITE").toInt();
        QDate dateAjout = q.value("DATE_P").toDate();

        QString probleme = "";

        // Anomalie 1 : Stock faible (moins de 10 unités)
        if (quantite < 10) {
            probleme = "Stock faible ! (< 10)";
        }

        // Anomalie 2 : Produit très ancien (plus de 180 jours)
        if (dateAjout.daysTo(QDate::currentDate()) > 180) {
            if (!probleme.isEmpty()) probleme += " | ";
            probleme += "Produit ancien (> 6 mois)";
        }

        // Anomalie 3 : Produit sans mouvement depuis longtemps + stock élevé = surstock dormant
        if (quantite > 50 && dateAjout.daysTo(QDate::currentDate()) > 120) {
            if (!probleme.isEmpty()) probleme += " | ";
            probleme += "Surstock dormant";
        }

        // Si au moins une anomalie détectée → afficher dans le tableau
        if (!probleme.isEmpty()) {
            ui->tableWidget_anomalies->insertRow(row);
            ui->tableWidget_anomalies->setItem(row, 0, new QTableWidgetItem(QString::number(ref)));
            ui->tableWidget_anomalies->setItem(row, 1, new QTableWidgetItem(marque));
            ui->tableWidget_anomalies->setItem(row, 2, new QTableWidgetItem(probleme));
            row++;
        }
    }

    // Optionnel : message si aucune anomalie
    if (row == 0) {
        ui->tableWidget_anomalies->insertRow(0);
        ui->tableWidget_anomalies->setItem(0, 0, new QTableWidgetItem("Aucune"));
        ui->tableWidget_anomalies->setItem(0, 1, new QTableWidgetItem("anomalie"));
        ui->tableWidget_anomalies->setItem(0, 2, new QTableWidgetItem("détectée"));
    }
}
// ===================== STATISTIQUES =====================
void MainWindow::afficherStatistiques()
{
    QPieSeries *series = new QPieSeries();
    series->setHoleSize(0.35);       // donut
    series->setPieSize(0.8);

    QSqlQuery query;
    query.exec("SELECT CATEGORIE, COUNT(*) FROM PRODUIT GROUP BY CATEGORIE");

    QMap<QString, int> stats;
    int total = 0;
    while (query.next()) {
        QString cat = query.value(0).toString();
        int nb     = query.value(1).toInt();
        stats[cat] = nb;
        total += nb;
    }

    // Couleurs personnalisées (orange / bleu / rouge)
    if (stats.value("lunettes") > 0) {
        QPieSlice *s = series->append("Lunettes", stats["lunettes"]);
        s->setBrush(QColor(255, 170, 0));
        s->setLabelVisible();
        s->setLabel(QString("Lunettes %1%").arg(100.0 * stats["lunettes"] / total, 0, 'f', 1));
    }
    if (stats.value("accessoires") > 0) {
        QPieSlice *s = series->append("Accessoires", stats["accessoires"]);
        s->setBrush(QColor(0, 150, 255));
        s->setLabelVisible();
        s->setLabel(QString("Accessoires %1%").arg(100.0 * stats["accessoires"] / total, 0, 'f', 1));
    }
    if (stats.value("lentilles") > 0) {
        QPieSlice *s = series->append("Lentilles", stats["lentilles"]);
        s->setBrush(QColor(255, 90, 90));
        s->setLabelVisible();
        s->setLabel(QString("Lentilles %1%").arg(100.0 * stats["lentilles"] / total, 0, 'f', 1));
    }

    if (total == 0) {
        series->append("Aucun produit", 1);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des produits par catégorie");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->setTheme(QChart::ChartThemeDark);
    chart->setTitleFont(QFont("Arial", 16, QFont::Bold));

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->resize(800, 600);
    chartView->setWindowTitle("Statistiques - Produits");
    chartView->show();
}
// ==================== FONCTIONS UTILITAIRES ====================
void MainWindow::viderChampsClients()
{
    ui->id_client->clear();
    ui->nom_client->clear();
    ui->prenom_client->clear();
    ui->mail_client->clear();
    ui->telephone_client->clear();
    ui->age_client->clear();
    ui->adresse_client->clear();
    ui->points_client->clear();
}

void MainWindow::viderChampsRDV()
{
    ui->lineEdit_43->clear();
    ui->lineEdit_48->clear();
    ui->lineEdit_45->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit_2->setTime(QTime::currentTime());
}

void MainWindow::viderChampsproduit()
{
    ui->refProduit->clear();
    ui->categorieProduit->setCurrentIndex(0);
    ui->marqueProduit->clear();
    ui->prixProduit->clear();
    ui->quantiteProduit->clear();

}



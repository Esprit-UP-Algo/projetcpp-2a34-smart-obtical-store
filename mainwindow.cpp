
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
, ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initialiserConnections();

    // Charger les données initiales
    chargerClients();
    chargerRDV();
    chargerProduits();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initialiserConnections()
{
    // ==================== NAVIGATION ====================
    // Menu principal (page_5)
    connect(ui->pushButton_11, &QPushButton::clicked, this, &MainWindow::afficherPageAccueil);
    connect(ui->pushButton_13, &QPushButton::clicked, this, &MainWindow::afficherPageProduits);
    connect(ui->pushButton_16, &QPushButton::clicked, this, &MainWindow::afficherPageRDV);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::afficherPageClients);

    // Menu RDV (page)
    connect(ui->acc, &QPushButton::clicked, this, &MainWindow::afficherPageAccueil);
    connect(ui->btnMenuProduit, &QPushButton::clicked, this, &MainWindow::afficherPageProduits);
    connect(ui->menubtnFournisseur, &QPushButton::clicked, this, &MainWindow::afficherPageRDV);
    connect(ui->client, &QPushButton::clicked, this, &MainWindow::afficherPageClients);

    // Menu Clients (page_2)
    connect(ui->pushButton_110, &QPushButton::clicked, this, &MainWindow::afficherPageAccueil);
    connect(ui->produit_bt, &QPushButton::clicked, this, &MainWindow::afficherPageProduits);
    connect(ui->rdv_bt, &QPushButton::clicked, this, &MainWindow::afficherPageRDV);
    connect(ui->client_bt, &QPushButton::clicked, this, &MainWindow::afficherPageClients);

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

    // ==================== PRODUITS ====================
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::ajouterProduit);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::modifierProduit);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &MainWindow::supprimerProduit);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &MainWindow::trierProduit);
    connect(ui->pushButton_9, &QPushButton::clicked, this, &MainWindow::exporterProduitsPDF);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::supprimerProduit);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &MainWindow::rechercherProduit);

    // ==================== RDV ====================
    connect(ui->pushButton_58, &QPushButton::clicked, this, &MainWindow::ajouterRDV);
    connect(ui->pushButton_59, &QPushButton::clicked, this, &MainWindow::modifierRDV);
    connect(ui->pushButton_60, &QPushButton::clicked, this, &MainWindow::supprimerRDV);
    connect(ui->pushButton_64, &QPushButton::clicked, this, &MainWindow::trierRDV);
    connect(ui->pushButton_63, &QPushButton::clicked, this, &MainWindow::exporterProduitsPDF);
    connect(ui->pushButton_62, &QPushButton::clicked, this, &MainWindow::supprimerRDV);
    connect(ui->pushButton_61, &QPushButton::clicked, this, &MainWindow::rechercherRDV);
    connect(ui->btnStatistiqueFournisseur, &QPushButton::clicked, this, &MainWindow::afficherStatistiquesRDV);
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged,
            this, &MainWindow::selectionProduitChange);

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

void MainWindow::afficherPageProduits() {
    ui->stackedWidget->setCurrentWidget(ui->page_5);
    chargerProduits();
}

void MainWindow::afficherPageRDV() {
    ui->stackedWidget->setCurrentWidget(ui->page);
    chargerRDV();
}

// ==================== FONCTIONS CLIENTS ====================
void MainWindow::ajouterClient()
{
    Client c;
    c.setId(ui->id_client->text().toInt());
    c.setNom(ui->nom_client->text());
    c.setPrenom(ui->prenom_client->text());
    c.setEmail(ui->mail_client->text());
    c.setTelephone(ui->telephone_client->text());
    c.setAge(ui->age_client->text().toInt());
    c.setAdresse(ui->adresse_client->text());
    c.setPointsFidelite(ui->points_client->text().toInt());

    QString errMsg;
    if (c.ajouter(&errMsg)) {
        QMessageBox::information(this, "Succès", "Client ajouté avec succès!");
        viderChampsClients();
        chargerClients();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec ajout: " + errMsg);
    }
}

void MainWindow::modifierClient()
{
    if (ui->id_client->text().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un client à modifier");
        return;
    }

    Client c;
    c.setId(ui->id_client->text().toInt());
    c.setNom(ui->nom_client->text());
    c.setPrenom(ui->prenom_client->text());
    c.setEmail(ui->mail_client->text());
    c.setTelephone(ui->telephone_client->text());
    c.setAge(ui->age_client->text().toInt());
    c.setAdresse(ui->adresse_client->text());
    c.setPointsFidelite(ui->points_client->text().toInt());

    QString errMsg;
    if (c.modifier(&errMsg)) {
        QMessageBox::information(this, "Succès", "Client modifié avec succès!");
        viderChampsClients();
        chargerClients();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec modification: " + errMsg);
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
    // ✅ Vérifier qu'il y a des clients
    if(ui->tableWidget_cl->rowCount() == 0){
        QMessageBox::warning(this, "PDF", "Aucun client à exporter !");
        return;
    }

    // ✅ Dialogue pour choisir où enregistrer
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer le PDF",
                                                    QDir::homePath() + "/clients_export.pdf",
                                                    "Fichiers PDF (*.pdf)");
    if(fileName.isEmpty()) return;

    // ✅ S'assurer que l'extension .pdf est bien présente
    if(!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // ✅ Configuration du printer
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QPainter painter;
    if (!painter.begin(&printer)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF !\n\nVérifiez que :\n• Le chemin d'accès existe\n• Vous avez les droits d'écriture\n• Le fichier n'est pas déjà ouvert");
        return;
    }

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // ============= FOND GRADIENT =============
    QLinearGradient bgGradient(0, 0, 0, printer.height());
    bgGradient.setColorAt(0.0, QColor("#FFF8E1"));
    bgGradient.setColorAt(0.3, QColor("#FFE0B2"));
    bgGradient.setColorAt(0.7, QColor("#FFCC80"));
    bgGradient.setColorAt(1.0, QColor("#FFB74D"));
    painter.setBrush(bgGradient);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, printer.width(), printer.height());

    // ============= BORDURE =============
    painter.setPen(QPen(QColor("#E65100"), 15));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(50, 50, printer.width() - 100, printer.height() - 100, 40, 40);

    int y = 180;

    // ============= LOGO =============
    painter.setBrush(QColor("#FF6F00"));
    painter.setPen(QPen(QColor("#E65100"), 10));
    painter.drawEllipse(printer.width()/2 - 80, y - 100, 160, 160);

    painter.setFont(QFont("Segoe UI", 48, QFont::Bold));
    painter.setPen(Qt::white);
    painter.drawText(printer.width()/2 - 80, y - 80, 160, 140, Qt::AlignCenter, "📋");

    // ============= TITRE =============
    painter.setFont(QFont("Georgia", 42, QFont::Bold));
    painter.setPen(QColor("#E65100"));
    painter.drawText(0, y + 20, printer.width(), 100, Qt::AlignCenter, "Liste des Clients");

    y += 140;

    // ============= LIGNE SÉPARATRICE =============
    painter.setPen(QPen(QColor("#FF6F00"), 8));
    painter.drawLine(200, y, printer.width() - 200, y);

    y += 60;

    // ============= DATE =============
    painter.setFont(QFont("Segoe UI", 18, QFont::Bold));
    painter.setBrush(QColor("#FFECB3"));
    painter.setPen(QPen(QColor("#FF8A65"), 4));
    painter.drawRoundedRect(printer.width()/2 - 300, y, 600, 70, 20, 20);

    painter.setPen(QColor("#BF360C"));
    QString dateStr = "📅 " + QDate::currentDate().toString("dddd dd MMMM yyyy");
    painter.drawText(printer.width()/2 - 280, y + 10, 560, 50, Qt::AlignCenter, dateStr);

    y += 120;

    // ============= EN-TÊTE TABLEAU =============
    QLinearGradient headerGradient(0, y, 0, y + 95);
    headerGradient.setColorAt(0.0, QColor("#FF8A65"));
    headerGradient.setColorAt(1.0, QColor("#FF5722"));
    painter.setBrush(headerGradient);
    painter.setPen(QPen(QColor("#BF360C"), 6));
    painter.drawRoundedRect(80, y, printer.width() - 160, 95, 25, 25);

    painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
    painter.setPen(Qt::white);
    QStringList headers = {"ID", "Nom", "Prénom", "Email", "Téléphone", "Âge", "Adresse", "Points"};

    int totalWidth = printer.width() - 200;
    QVector<int> colWidths = {
        int(totalWidth * 0.10),
        int(totalWidth * 0.12),
        int(totalWidth * 0.12),
        int(totalWidth * 0.20),
        int(totalWidth * 0.13),
        int(totalWidth * 0.08),
        int(totalWidth * 0.16),
        int(totalWidth * 0.09)
    };

    int xPos = 95;
    for(int j = 0; j < 8; ++j) {
        painter.drawText(xPos, y + 20, colWidths[j], 65, Qt::AlignCenter, headers[j]);
        xPos += colWidths[j];
    }
    y += 115;

    // ============= LIGNES DU TABLEAU =============
    painter.setFont(QFont("Segoe UI", 11));
    int totalRows = ui->tableWidget_cl->rowCount();

    for(int i = 0; i < totalRows; ++i) {
        // ⚠️ Nouvelle page si nécessaire
        if (y > printer.height() - 550) {
            // Footer avant nouvelle page
            int footerY = printer.height() - 280;
            painter.setBrush(QColor("#FF8A65"));
            painter.setPen(QPen(QColor("#E65100"), 8));
            painter.drawRoundedRect(100, footerY, printer.width() - 200, 180, 35, 35);
            painter.setFont(QFont("Segoe UI", 18, QFont::Bold));
            painter.setPen(Qt::white);
            painter.drawText(0, footerY + 70, printer.width(), 50, Qt::AlignCenter, "Document Généré Automatiquement");

            // Nouvelle page
            printer.newPage();

            // Redessiner fond
            painter.setBrush(bgGradient);
            painter.setPen(Qt::NoPen);
            painter.drawRect(0, 0, printer.width(), printer.height());
            painter.setPen(QPen(QColor("#E65100"), 15));
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundedRect(50, 50, printer.width() - 100, printer.height() - 100, 40, 40);

            y = 150;

            // Re-dessiner header
            painter.setBrush(headerGradient);
            painter.setPen(QPen(QColor("#BF360C"), 6));
            painter.drawRoundedRect(80, y, printer.width() - 160, 95, 25, 25);
            painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
            painter.setPen(Qt::white);

            xPos = 95;
            for(int j = 0; j < 8; ++j) {
                painter.drawText(xPos, y + 20, colWidths[j], 65, Qt::AlignCenter, headers[j]);
                xPos += colWidths[j];
            }
            y += 115;
        }

        // Ligne de données
        QColor rowColor = (i % 2 == 0) ? QColor("#FFFFFF") : QColor("#FFF3E0");
        painter.setBrush(rowColor);
        painter.setPen(QPen(QColor("#FFCC80"), 3));
        painter.drawRoundedRect(80, y, printer.width() - 160, 80, 18, 18);

        painter.setBrush(QColor("#FF6F00"));
        painter.drawRoundedRect(80, y, 10, 80, 5, 5);

        painter.setFont(QFont("Segoe UI", 10));
        painter.setPen(QColor("#D84315"));

        xPos = 95;
        for(int j = 0; j < 8; ++j) {
            QTableWidgetItem *item = ui->tableWidget_cl->item(i, j);
            QString text = item ? item->text() : "";

            QFontMetrics fm(painter.font());
            if (fm.horizontalAdvance(text) > colWidths[j] - 30) {
                text = fm.elidedText(text, Qt::ElideRight, colWidths[j] - 30);
            }

            Qt::Alignment align = (j == 0 || j == 5 || j == 7) ? Qt::AlignCenter : Qt::AlignLeft | Qt::AlignVCenter;
            painter.drawText(xPos + 5, y + 10, colWidths[j] - 10, 60, align, text);
            xPos += colWidths[j];
        }

        y += 90;
    }

    // ============= FOOTER FINAL =============
    int footerY = printer.height() - 280;
    painter.setBrush(QColor("#FF8A65"));
    painter.setPen(QPen(QColor("#E65100"), 8));
    painter.drawRoundedRect(100, footerY, printer.width() - 200, 180, 35, 35);
    painter.setFont(QFont("Segoe UI", 18, QFont::Bold));
    painter.setPen(Qt::white);
    painter.drawText(0, footerY + 70, printer.width(), 50, Qt::AlignCenter, "Document Généré Automatiquement");

    painter.end();

    // ✅ CONFIRMATION
    QMessageBox::information(this, "✅ Succès",
                             QString("PDF exporté avec succès !\n\n📄 %1\n\n📊 %2 clients exportés")
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
    // ✅ VÉRIFICATION 1: Le tabWidget existe-t-il ?
    if (!ui->tabWidget_13) {
        qDebug() << "❌ tabWidget_13 est NULL !";
        return;
    }

    // ✅ VÉRIFICATION 2: L'onglet statistiques existe-t-il ?
    QWidget *statsTab = ui->tabWidget_13->widget(1);
    if (!statsTab) {
        qDebug() << "❌ L'onglet statistiques (index 1) n'existe pas !";
        qDebug() << "Nombre d'onglets :" << ui->tabWidget_13->count();
        return;
    }

    // ✅ NETTOYAGE COMPLET: Layout + ancien graphique
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

    // ✅ GÉNÉRATION DU PIE CHART
    const int size = 400;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    QRectF rect(20, 20, size - 40, size - 40);

    QVector<QColor> colors = {
        QColor("#e74c3c"),  // Rouge
        QColor("#3498db"),  // Bleu
        QColor("#2ecc71"),  // Vert
        QColor("#f1c40f")   // Jaune
    };
    QVector<int> values = {c12_18, c19_30, c31_50, c50_plus};
    QVector<QString> labels = {"12-18 ans", "19-30 ans", "31-50 ans", "50+ ans"};

    int startAngle = 90 * 16;

    for (int i = 0; i < 4; ++i) {
        if (values[i] == 0) continue;

        int spanAngle = qRound(360.0 * 16 * values[i] / total);
        int percent = qRound(100.0 * values[i] / total);

        // Dessiner la tranche
        painter.setBrush(colors[i]);
        painter.setPen(QPen(Qt::white, 3));
        painter.drawPie(rect, startAngle, spanAngle);

        // Calculer position texte
        double midAngle = (startAngle + spanAngle / 2.0) / 16.0;
        double rad = qDegreesToRadians(midAngle);
        double radius = size * 0.32;

        int cx = size / 2 + radius * qCos(rad);
        int cy = size / 2 - radius * qSin(rad);

        // Label avec contour noir
        painter.setFont(QFont("Segoe UI", 11, QFont::Bold));
        painter.setPen(QPen(Qt::black, 2));
        painter.drawText(cx - 60, cy - 10, 120, 25, Qt::AlignCenter, labels[i]);

        // Label en blanc
        painter.setPen(Qt::white);
        painter.drawText(cx - 60, cy - 10, 120, 25, Qt::AlignCenter, labels[i]);

        // Pourcentage
        painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
        painter.setPen(QPen(Qt::black, 2));
        painter.drawText(cx - 40, cy + 15, 80, 30, Qt::AlignCenter, QString("%1%").arg(percent));

        painter.setPen(Qt::white);
        painter.drawText(cx - 40, cy + 15, 80, 30, Qt::AlignCenter, QString("%1%").arg(percent));

        startAngle += spanAngle;
    }

    // ⭐ AFFICHAGE AVEC LAYOUT POUR CENTRAGE PARFAIT ⭐
    QVBoxLayout *layout = new QVBoxLayout(statsTab);
    layout->setAlignment(Qt::AlignCenter);
    layout->setContentsMargins(20, 20, 20, 20);


    // Le graphique
    QLabel *label = new QLabel();
    label->setObjectName("pieChartLabel");
    label->setPixmap(pixmap);
    label->setFixedSize(size, size);
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "background-color: white; "
        "border: 2px solid #e0e0e0; "
        "border-radius: 15px; "
        "padding: 10px;"
        );

    layout->addWidget(label);

    qDebug() << "✅ Pie Chart centré avec succès !";
    qDebug() << "   Total clients:" << total;
    qDebug() << "   12-18:" << c12_18 << "| 19-30:" << c19_30
             << "| 31-50:" << c31_50 << "| 50+:" << c50_plus;
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
// ==================== FONCTIONS PRODUITS ====================
// ==================== FONCTIONS PRODUITS ====================
void MainWindow::ajouterProduit()
{
    Produit p;
    p.setReference(ui->lineEdit->text().toInt());
    p.setCategorie(ui->comboBox->currentText());
    p.setMarque(ui->lineEdit_2->text());
    p.setPrix(ui->lineEdit_3->text().toDouble());
    p.setQuantite(ui->lineEdit_4->text().toInt());

    if (p.ajouter()) {
        QMessageBox::information(this, "Succès", "Produit ajouté avec succès!");
        viderChampsProduits();
        chargerProduits();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec d'ajout du produit");
    }
}

void MainWindow::modifierProduit()
{
    if (ui->lineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un produit à modifier");
        return;
    }

    Produit p;
    p.setReference(ui->lineEdit->text().toInt());
    p.setCategorie(ui->comboBox->currentText());
    p.setMarque(ui->lineEdit_2->text());
    p.setPrix(ui->lineEdit_3->text().toDouble());
    p.setQuantite(ui->lineEdit_4->text().toInt());

    if (p.modifier()) {
        QMessageBox::information(this, "Succès", "Produit modifié avec succès!");
        viderChampsProduits();
        chargerProduits();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de modification du produit");
    }
}

void MainWindow::supprimerProduit()
{
    if (ui->lineEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un produit à supprimer");
        return;
    }

    auto reponse = QMessageBox::question(this, "Confirmation",
                                         "Voulez-vous vraiment supprimer ce produit?",
                                         QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::Yes) {
        if (Produit::supprimer(ui->lineEdit->text().toInt())) {
            QMessageBox::information(this, "Succès", "Produit supprimé!");
            viderChampsProduits();
            chargerProduits();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de suppression du produit");
        }
    }
}

void MainWindow::rechercherProduit()
{
    QString critere = ui->lineEdit_9->text();
    QSqlQueryModel *model = Produit::rechercher(critere);

    // Pour QTableWidget, on doit peupler manuellement
    ui->tableWidget->setRowCount(0);

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(
                model->data(model->index(row, col)).toString()
                );
            ui->tableWidget->setItem(row, col, item);
        }
    }
}

void MainWindow::trierProduit()
{
    QString critere = ui->comboBox_3->currentText();
    QString ordre = ui->radioButton->isChecked() ? "ASC" : "DESC";

    QSqlQueryModel *model = Produit::trier(critere, ordre);

    // Pour QTableWidget, on doit peupler manuellement
    ui->tableWidget->setRowCount(0);

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(
                model->data(model->index(row, col)).toString()
                );
            ui->tableWidget->setItem(row, col, item);
        }
    }
}

void MainWindow::exporterProduitsPDF()
{
    QString filepath = QFileDialog::getSaveFileName(this, "Exporter PDF", "", "PDF Files (*.pdf)");
    if (!filepath.isEmpty()) {
        if (Produit::exporterPDF(filepath)) {
            QMessageBox::information(this, "Succès", "Export PDF réussi!");
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de l'export PDF");
        }
    }
}

void MainWindow::chargerProduits()
{
    QSqlQueryModel *model = Produit::afficher();

    // Pour QTableWidget, on doit peupler manuellement
    ui->tableWidget->setRowCount(0);

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < model->columnCount(); ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(
                model->data(model->index(row, col)).toString()
                );
            ui->tableWidget->setItem(row, col, item);
        }
    }
}

void MainWindow::selectionProduitChange()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->selectedItems();
    if (!items.isEmpty()) {
        int row = items[0]->row();
        ui->lineEdit->setText(ui->tableWidget->item(row, 0)->text());
        ui->comboBox->setCurrentText(ui->tableWidget->item(row, 1)->text());
        ui->lineEdit_2->setText(ui->tableWidget->item(row, 2)->text());
        ui->lineEdit_3->setText(ui->tableWidget->item(row, 3)->text());
        ui->lineEdit_4->setText(ui->tableWidget->item(row, 4)->text());
    }
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

void MainWindow::viderChampsProduits()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->comboBox->setCurrentIndex(0);
}



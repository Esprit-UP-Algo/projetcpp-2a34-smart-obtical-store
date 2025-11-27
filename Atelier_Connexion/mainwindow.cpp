#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "client.h"
#include "connection.h"
#include "qrcodegenerator.h"

#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>
#include <QPrinter>
#include <QTextDocument>
#include <QDebug>
#include <QPainter>
#include <QLabel>
#include <QtMath>
#include <QTimer>



// ==================== CONSTRUCTEUR ====================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialisation du tri
    currentOrderBy = "NOMC";
    currentAsc = true;
    ui->radioCroissant->setChecked(true);

    // Initialisation des combobox
    ui->comboBox_2->clear();
    ui->comboBox_2->addItems({"ID", "telephone", "mail"});

    ui->trier_combo->clear();
    ui->trier_combo->addItems({"Nom", "Âge", "Points fidélité"});
    connect(ui->btnGenerateQR, &QPushButton::clicked, this, &MainWindow::generateQR);
    connect(ui->btnSaveQR,     &QPushButton::clicked, this, &MainWindow::saveQR);
    connect(ui->btnCopyQR,     &QPushButton::clicked, this, &MainWindow::copyQR);
    if (!ui->labelQrCode)    qFatal("labelQrCode est NULL → mauvais nom dans le .ui !");
    if (!ui->btnGenerateQR)  qFatal("btnGenerateQR est NULL → mauvais nom !");
    if (!ui->btnCopyQR)      qFatal("btnCopyQR est NULL → mauvais nom !");
    if (!ui->btnSaveQR)      qFatal("btnSaveQR est NULL → mauvais nom !");
    // Affichage initial
    refreshClients();  // → mettra aussi à jour le Pie Chart
}

MainWindow::~MainWindow() { delete ui; }

// ==================== LECTURE FORMULAIRE ====================
bool MainWindow::readForm(int &id, QString &nom, QString &prenom, QString &mail,
                          QString &tel, int &age, QString &adresse, int &points)
{
    id      = ui->id_client->text().toInt();
    nom     = ui->nom_client->text().trimmed();
    prenom  = ui->prenom_client->text().trimmed();
    mail    = ui->mail_client->text().trimmed();
    tel     = ui->telephone_client->text().trimmed();
    age     = ui->age_client->text().toInt();
    adresse = ui->adresse_client->text().trimmed();
    points  = ui->points_client->text().toInt();

    Client c;
    c.setId(id); c.setNom(nom); c.setPrenom(prenom);
    c.setEmail(mail); c.setTelephone(tel);
    c.setAge(age); c.setAdresse(adresse);
    c.setPointsFidelite(points);

    if(!c.isValidId()){
        QMessageBox::warning(this,"Erreur","ID : 6 chiffres !");
        return false;
    }
    if(!c.isValidAge()){
        QMessageBox::warning(this,"Erreur","Âge : max 3 chiffres !");
        return false;
    }
    if(!c.isValidPoints()){
        QMessageBox::warning(this,"Erreur","Points : 1 à 4 chiffres !");
        return false;
    }
    if(!c.isValidTelephone()){
        QMessageBox::warning(this,"Erreur","Téléphone : 8 chiffres !");
        return false;
    }
    if(!c.isValidEmail()){
        QMessageBox::warning(this,"Erreur","Email invalide !");
        return false;
    }

    return true;
}

// ==================== CRUD ====================

void MainWindow::on_ajouter_bt_clicked()
{
    int id, age, points;
    QString nom, prenom, mail, tel, adresse;
    if(!readForm(id, nom, prenom, mail, tel, age, adresse, points)) return;

    Client c;
    c.setId(id);
    c.setNom(nom);
    c.setPrenom(prenom);
    c.setEmail(mail);
    c.setTelephone(tel);
    c.setAge(age);
    c.setAdresse(adresse);
    c.setPointsFidelite(points);

    QString err;
    if(c.ajouter(&err)){
        QMessageBox::information(this,"Succès","Client ajouté ✅");
        refreshClients();
    } else {
        QMessageBox::critical(this,"Erreur",err);
    }
}

void MainWindow::on_modifier_bt_clicked()
{
    int id, age, points;
    QString nom, prenom, mail, tel, adresse;
    if(!readForm(id, nom, prenom, mail, tel, age, adresse, points)) return;

    Client c;
    c.setId(id);
    c.setNom(nom);
    c.setPrenom(prenom);
    c.setEmail(mail);
    c.setTelephone(tel);
    c.setAge(age);
    c.setAdresse(adresse);
    c.setPointsFidelite(points);

    QString err;
    if(c.modifier(&err)){
        QMessageBox::information(this,"Succès","Client modifié ✅");
        refreshClients();
    } else {
        QMessageBox::critical(this,"Erreur",err);
    }
}

void MainWindow::on_supprimer_bt_clicked()
{
    int row = ui->tableWidget_cl->currentRow();
    if(row < 0){
        QMessageBox::warning(this,"Erreur","Sélectionnez un client !");
        return;
    }

    QTableWidgetItem *item = ui->tableWidget_cl->item(row, 0);
    if(!item){
        QMessageBox::warning(this,"Erreur","Impossible de lire l'ID !");
        return;
    }

    bool ok = false;
    int id = item->text().toInt(&ok);
    if(!ok || id <= 0){
        QMessageBox::warning(this,"Erreur","ID invalide !");
        return;
    }

    int reply = QMessageBox::question(this, "Confirmation",
                                      QString("Supprimer le client %1 ?").arg(id),
                                      QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No) return;

    QString err;
    if(Client::supprimer(id, &err)){
        QMessageBox::information(this,"Succès","Client supprimé ✅");

        ui->id_client->clear();
        ui->nom_client->clear();
        ui->prenom_client->clear();
        ui->mail_client->clear();
        ui->telephone_client->clear();
        ui->age_client->clear();
        ui->adresse_client->clear();
        ui->points_client->clear();

        refreshClients();
    } else {
        QMessageBox::critical(this,"Erreur",err);
    }
}

// ==================== PIE CHART ====================
// (fonction ajoutée)
void MainWindow::afficherPieChart(int c12_18, int c19_30, int c31_50, int c50_plus, int total)
{
    if (auto old = ui->tabWidget_13->findChild<QLabel*>("pieChartLabel"))
        old->deleteLater();

    QWidget *statsTab = ui->tabWidget_13->widget(1);

    if (total == 0) {
        QLabel *lbl = new QLabel(statsTab);
        lbl->setObjectName("pieChartLabel");
        lbl->setText("Aucun client");
        lbl->setAlignment(Qt::AlignCenter);
        lbl->setStyleSheet("font-size: 28px; color: #c0392b; font-weight: bold;");
        lbl->setGeometry(0, 0, statsTab->width(), statsTab->height());
        lbl->show();
        return;
    }

    const int size = 400;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);

    QRectF rect(20, 20, size - 40, size - 40);

    QVector<QColor> colors = { "#e74c3c", "#3498db", "#2ecc71", "#f1c40f" };
    QVector<int> values = {c12_18, c19_30, c31_50, c50_plus};
    QVector<QString> labels = {"12-18 ans", "19-30 ans", "31-50 ans", "50+ ans"};

    int startAngle = 90 * 16;

    for (int i = 0; i < 4; ++i) {
        if (values[i] == 0) continue;

        int spanAngle = qRound(360.0 * 16 * values[i] / total);
        int percent = qRound(100.0 * values[i] / total);

        // رسم القطعة
        painter.setBrush(colors[i]);
        painter.setPen(QPen(Qt::white, 10));
        painter.drawPie(rect, startAngle, spanAngle);

        // حساب الموقع داخل القطعة
        double midAngle = (startAngle + spanAngle / 2.0) / 16.0;
        double rad = qDegreesToRadians(midAngle);
        double radius = size * 0.32;  // قريب من المركز

        int cx = size / 2 + radius * cos(rad);
        int cy = size / 2 - radius * sin(rad);

        // الفئة العمرية (حجم مناسب وواضح)
        painter.setFont(QFont("Segoe UI", 12, QFont::Bold));
        painter.setPen(QPen(Qt::black, 2.5));
        painter.drawText(cx - 80, cy - 12, 160, 30, Qt::AlignCenter, labels[i]);
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawText(cx - 78, cy - 10, 156, 26, Qt::AlignCenter, labels[i]);

        // النسبة المئوية (صغيرة وتحت الفئة بوضوح)
        painter.setFont(QFont("Segoe UI", 16, QFont::Bold));  // صغرناها من 18 إلى 16
        painter.setPen(QPen(Qt::black, 3));
        painter.drawText(cx - 50, cy + 22, 100, 35, Qt::AlignCenter, QString("%1%").arg(percent));
        painter.setPen(Qt::NoPen);
        painter.setBrush(Qt::white);
        painter.drawText(cx - 48, cy + 24, 96, 31, Qt::AlignCenter, QString("%1%").arg(percent));

        startAngle += spanAngle;
    }

    // عنوان الدائرة


    QLabel *label = new QLabel(statsTab);
    label->setObjectName("pieChartLabel");
    label->setPixmap(pixmap);
    label->setFixedSize(size, size);
    label->move((statsTab->width() - size)/2, (statsTab->height() - size)/2 + 40);
    label->raise();
    label->show();
}
// ==================== TABLE / TRI / RECHERCHE ====================
void MainWindow::on_tableWidget_cl_itemClicked(QTableWidgetItem *item)
{
    int r = item->row();
    if(r<0) return;

    ui->id_client->setText(ui->tableWidget_cl->item(r,0)->text());
    ui->nom_client->setText(ui->tableWidget_cl->item(r,1)->text());
    ui->prenom_client->setText(ui->tableWidget_cl->item(r,2)->text());
    ui->mail_client->setText(ui->tableWidget_cl->item(r,3)->text());
    ui->telephone_client->setText(ui->tableWidget_cl->item(r,4)->text());
    ui->age_client->setText(ui->tableWidget_cl->item(r,5)->text());
    ui->adresse_client->setText(ui->tableWidget_cl->item(r,6)->text());
    ui->points_client->setText(ui->tableWidget_cl->item(r,7)->text());
}

void MainWindow::on_tableWidget_cl_currentCellChanged(int currentRow, int, int, int)
{
    if(currentRow < 0) return;

    ui->id_client->setText(ui->tableWidget_cl->item(currentRow,0)->text());
    ui->nom_client->setText(ui->tableWidget_cl->item(currentRow,1)->text());
    ui->prenom_client->setText(ui->tableWidget_cl->item(currentRow,2)->text());
    ui->mail_client->setText(ui->tableWidget_cl->item(currentRow,3)->text());
    ui->telephone_client->setText(ui->tableWidget_cl->item(currentRow,4)->text());
    ui->age_client->setText(ui->tableWidget_cl->item(currentRow,5)->text());
    ui->adresse_client->setText(ui->tableWidget_cl->item(currentRow,6)->text());
    ui->points_client->setText(ui->tableWidget_cl->item(currentRow,7)->text());
}

void MainWindow::on_trier_bt_clicked()
{
    QString choix = ui->trier_combo->currentText();
    if(choix=="Nom") currentOrderBy="NOMC";
    else if(choix=="Âge") currentOrderBy="AGEC";
    else if(choix=="Points fidélité") currentOrderBy="POINTS_CLIENTS";
    refreshClients();
}

void MainWindow::on_radioCroissant_toggled(bool checked){
    if(checked) {
        currentAsc=true;

    }
}

void MainWindow::on_radioDecroissant_toggled(bool checked){
    if(checked) {
        currentAsc=false;

    }
}

void MainWindow::on_recherche_textChanged(const QString &){
    refreshClients();
}

void MainWindow::on_comboBox_2_currentIndexChanged(int){
    refreshClients();
}

// ==================== EXPORT PDF ====================
void MainWindow::on_pdf_bt_clicked()
{
    if(ui->tableWidget_cl->rowCount() == 0){
        QMessageBox::warning(this,"PDF","Aucun client à exporter !");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", "", "Fichiers PDF (*.pdf)");
    if(fileName.isEmpty()) return;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize::A4);
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);

    // ============= FOND GRADIENT ORANGE-BEIGE LUXUEUX =============
    QLinearGradient bgGradient(0, 0, 0, printer.height());
    bgGradient.setColorAt(0.0, QColor("#FFF8E1"));  // Beige clair
    bgGradient.setColorAt(0.3, QColor("#FFE0B2"));  // Beige orangé
    bgGradient.setColorAt(0.7, QColor("#FFCC80"));  // Orange doux
    bgGradient.setColorAt(1.0, QColor("#FFB74D"));  // Orange riche
    painter.setBrush(bgGradient);
    painter.setPen(Qt::NoPen);
    painter.drawRect(0, 0, printer.width(), printer.height());

    // ============= BORDURE DÉCORATIVE =============
    painter.setPen(QPen(QColor("#E65100"), 15));
    painter.setBrush(Qt::NoBrush);
    painter.drawRoundedRect(50, 50, printer.width() - 100, printer.height() - 100, 40, 40);

    painter.setPen(QPen(QColor("#FF8A65"), 8));
    painter.drawRoundedRect(70, 70, printer.width() - 140, printer.height() - 140, 30, 30);

    int y = 180;

    // ============= LOGO CERCLE DÉCORATIF =============
    painter.setBrush(QColor("#FF6F00"));
    painter.setPen(QPen(QColor("#E65100"), 10));
    painter.drawEllipse(printer.width()/2 - 80, y - 100, 160, 160);

    painter.setFont(QFont("Segoe UI", 48, QFont::Bold));
    painter.setPen(Qt::white);
    painter.drawText(printer.width()/2 - 80, y - 80, 160, 140, Qt::AlignCenter, "📋");

    // ============= TITRE PRINCIPAL =============
    painter.setFont(QFont("Georgia", 42, QFont::Bold));
    QLinearGradient titleGradient(0, y + 20, 0, y + 100);
    titleGradient.setColorAt(0.0, QColor("#E65100"));
    titleGradient.setColorAt(1.0, QColor("#BF360C"));
    painter.setPen(QPen(QBrush(titleGradient), 2));
    painter.drawText(0, y + 20, printer.width(), 100, Qt::AlignCenter, "Liste des Clients");

    y += 140;

    // ============= LIGNE SÉPARATRICE ÉLÉGANTE =============
    QLinearGradient lineGradient(200, y, printer.width() - 200, y);
    lineGradient.setColorAt(0.0, QColor("#FFE0B2"));
    lineGradient.setColorAt(0.5, QColor("#FF6F00"));
    lineGradient.setColorAt(1.0, QColor("#FFE0B2"));
    painter.setPen(QPen(QBrush(lineGradient), 8));
    painter.drawLine(200, y, printer.width() - 200, y);

    y += 60;

    // ============= DATE STYLISÉE =============
    painter.setFont(QFont("Segoe UI", 18, QFont::Bold));
    painter.setPen(QColor("#D84315"));

    // Cadre décoratif pour la date
    painter.setBrush(QColor("#FFECB3"));
    painter.setPen(QPen(QColor("#FF8A65"), 4));
    painter.drawRoundedRect(printer.width()/2 - 300, y, 600, 70, 20, 20);

    painter.setPen(QColor("#BF360C"));
    QString dateStr = "📅 " + QDate::currentDate().toString("dddd dd MMMM yyyy");
    painter.drawText(printer.width()/2 - 280, y + 10, 560, 50, Qt::AlignCenter, dateStr);

    y += 120;

    // ============= EN-TÊTE DU TABLEAU ULTRA LUXE =============
    // Ombre portée
    painter.setBrush(QColor(0, 0, 0, 40));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(90, y + 15, printer.width() - 180, 95, 25, 25);

    // Gradient principal de l'en-tête
    QLinearGradient headerGradient(0, y, 0, y + 95);
    headerGradient.setColorAt(0.0, QColor("#FF8A65"));
    headerGradient.setColorAt(0.5, QColor("#FF7043"));
    headerGradient.setColorAt(1.0, QColor("#FF5722"));
    painter.setBrush(headerGradient);
    painter.setPen(QPen(QColor("#BF360C"), 6));
    painter.drawRoundedRect(80, y, printer.width() - 160, 95, 25, 25);

    // Reflets brillants
    QLinearGradient shine(0, y, 0, y + 40);
    shine.setColorAt(0.0, QColor(255, 255, 255, 60));
    shine.setColorAt(1.0, QColor(255, 255, 255, 0));
    painter.setBrush(shine);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(80, y, printer.width() - 160, 40, 25, 25);

    // Textes de l'en-tête avec largeurs personnalisées
    painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
    painter.setPen(Qt::white);
    QStringList headers = {"ID", "Nom", "Prénom", "Email", "Téléphone", "Âge", "Adresse", "Points"};

    // Largeurs OPTIMISÉES pour tenir dans la page A4 (total ~7000)
    int totalWidth = printer.width() - 200;  // Largeur disponible
    QVector<int> colWidths = {
        int(totalWidth * 0.10),   // ID - 10%
        int(totalWidth * 0.12),   // Nom - 12%
        int(totalWidth * 0.12),   // Prénom - 12%
        int(totalWidth * 0.20),   // Email - 20% (plus large)
        int(totalWidth * 0.13),   // Téléphone - 13%
        int(totalWidth * 0.08),   // Âge - 8%
        int(totalWidth * 0.16),   // Adresse - 16% (plus large)
        int(totalWidth * 0.09)    // Points - 9%
    };

    int xPos = 95;
    for(int j = 0; j < 8; ++j) {
        painter.drawText(xPos, y + 20, colWidths[j], 65, Qt::AlignCenter, headers[j]);
        xPos += colWidths[j];
    }
    y += 115;

    // ============= LIGNES DU TABLEAU =============
    painter.setFont(QFont("Segoe UI", 11));  // Police plus petite pour tenir

    for(int i = 0; i < ui->tableWidget_cl->rowCount(); ++i) {
        // Alternance de couleurs
        QColor rowColor = (i % 2 == 0) ? QColor("#FFFFFF") : QColor("#FFF3E0");

        // Ombre de ligne
        painter.setBrush(QColor(0, 0, 0, 25));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(88, y + 10, printer.width() - 176, 80, 18, 18);

        // Ligne principale
        painter.setBrush(rowColor);
        painter.setPen(QPen(QColor("#FFCC80"), 3));
        painter.drawRoundedRect(80, y, printer.width() - 160, 80, 18, 18);

        // Bordure gauche colorée (indicateur)
        painter.setBrush(QColor("#FF6F00"));
        painter.drawRoundedRect(80, y, 10, 80, 5, 5);

        // Texte des données avec espacement correct
        painter.setFont(QFont("Segoe UI", 10));  // Police encore plus petite
        painter.setPen(QColor("#D84315"));

        xPos = 95;
        for(int j = 0; j < 8; ++j) {
            QTableWidgetItem *item = ui->tableWidget_cl->item(i, j);
            QString text = item ? item->text() : "";

            // Tronquer les textes trop longs
            QFontMetrics fm(painter.font());
            if (fm.horizontalAdvance(text) > colWidths[j] - 30) {
                text = fm.elidedText(text, Qt::ElideRight, colWidths[j] - 30);
            }

            // Colonnes numériques centrées, texte aligné à gauche avec padding
            Qt::Alignment align;
            int paddingLeft = 5;

            if (j == 0 || j == 5 || j == 7) {  // ID, Âge, Points → Centre
                align = Qt::AlignCenter;
                paddingLeft = 0;
            } else if (j == 4) {  // Téléphone → Centre aussi
                align = Qt::AlignCenter;
                paddingLeft = 0;
            } else {  // Nom, Prénom, Email, Adresse → Gauche avec padding
                align = Qt::AlignLeft | Qt::AlignVCenter;
            }

            painter.drawText(xPos + paddingLeft, y + 10, colWidths[j] - 10, 60, align, text);
            xPos += colWidths[j];
        }
        y += 90;

        // Gestion des pages
        if (y > printer.height() - 400) {
            printer.newPage();

            // Redessiner le fond
            painter.setBrush(bgGradient);
            painter.setPen(Qt::NoPen);
            painter.drawRect(0, 0, printer.width(), printer.height());

            // Redessiner les bordures
            painter.setPen(QPen(QColor("#E65100"), 15));
            painter.setBrush(Qt::NoBrush);
            painter.drawRoundedRect(50, 50, printer.width() - 100, printer.height() - 100, 40, 40);

            y = 150;

            // Re-dessiner l'en-tête avec largeurs correctes
            painter.setBrush(headerGradient);
            painter.setPen(QPen(QColor("#BF360C"), 6));
            painter.drawRoundedRect(80, y, printer.width() - 160, 95, 25, 25);
            painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
            painter.setPen(Qt::white);

            // Recalculer les largeurs pour la nouvelle page
            totalWidth = printer.width() - 200;
            colWidths = {
                int(totalWidth * 0.10),
                int(totalWidth * 0.12),
                int(totalWidth * 0.12),
                int(totalWidth * 0.20),
                int(totalWidth * 0.13),
                int(totalWidth * 0.08),
                int(totalWidth * 0.16),
                int(totalWidth * 0.09)
            };

            xPos = 95;
            for(int j = 0; j < 8; ++j) {
                painter.drawText(xPos, y + 20, colWidths[j], 65, Qt::AlignCenter, headers[j]);
                xPos += colWidths[j];
            }
            y += 115;
        }
    }

    // ============= PIED DE PAGE LUXUEUX =============
    y = printer.height() - 300;

    // Cadre principal du footer
    QLinearGradient footerGradient(0, y, 0, y + 200);
    footerGradient.setColorAt(0.0, QColor("#FF8A65"));
    footerGradient.setColorAt(1.0, QColor("#FF7043"));
    painter.setBrush(footerGradient);
    painter.setPen(QPen(QColor("#E65100"), 8));
    painter.drawRoundedRect(100, y, printer.width() - 200, 200, 35, 35);

    // Reflet brillant
    QLinearGradient footerShine(0, y, 0, y + 60);
    footerShine.setColorAt(0.0, QColor(255, 255, 255, 50));
    footerShine.setColorAt(1.0, QColor(255, 255, 255, 0));
    painter.setBrush(footerShine);
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(100, y, printer.width() - 200, 60, 35, 35);

    // Icône décorative
    painter.setFont(QFont("Segoe UI", 32));
    painter.setPen(Qt::white);
    painter.drawText(0, y + 20, printer.width(), 70, Qt::AlignCenter, "✨");

    // Texte principal
    painter.setFont(QFont("Segoe UI", 20, QFont::Bold));
    painter.setPen(Qt::white);
    painter.drawText(0, y + 80, printer.width(), 60, Qt::AlignCenter, "Document Généré Automatiquement");

    // Sous-texte
    painter.setFont(QFont("Segoe UI", 14));
    painter.setPen(QColor("#FFECB3"));
    painter.drawText(0, y + 135, printer.width(), 50, Qt::AlignCenter, "© 2025 - Système Premium de Gestion des Clients");

    painter.end();

    QMessageBox::information(this, "✅ Succès",
                             "PDF exporté avec succès!\n\n📄 " + fileName );
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

    // الحل السحري: نرسم الدائرة دائمًا بدون شرط
    // حتى لو الـ Tab مش مفتوح → تبقى جاهزة وتظهر فورًا لما نفتحه
    afficherPieChart(c12_18, c19_30, c31_50, c50_plus, rows);

    // إضافة أمان: إذا كنا حاليًا في Tab الإحصائيات → نعيد رسمها تاني (للتحديث الفوري)
    if (ui->tabWidget_13->currentIndex() == 1) {
        QTimer::singleShot(50, this, [this, c12_18, c19_30, c31_50, c50_plus, rows]() {
            afficherPieChart(c12_18, c19_30, c31_50, c50_plus, rows);
        });
    }
}
void MainWindow::generateQR()
{
    int row = ui->tableWidget_cl->currentRow();
    if (row < 0) {
        QMessageBox::warning(this, "Attention", "Sélectionnez un client !");
        return;
    }

    // Créer l'objet ClientQR correctement
    ClientQR c;
    c.id = ui->tableWidget_cl->item(row, 0)->text().toInt();
    c.nom = ui->tableWidget_cl->item(row, 1)->text();
    c.prenom = ui->tableWidget_cl->item(row, 2)->text();
    c.telephone = ui->tableWidget_cl->item(row, 4)->text();
    c.points = ui->tableWidget_cl->item(row, 7)->text().toInt();

    // Génération du QR code
    QPixmap card = QRCodeGenerator::generate(c);

    // Affichage
    if (ui->labelQrCode) {
        ui->labelQrCode->setPixmap(card.scaled(ui->labelQrCode->size(),
                                               Qt::KeepAspectRatio,
                                               Qt::SmoothTransformation));
    }

    // Rafraîchir l'affichage des points
    refreshClients();
}
void MainWindow::saveQR()
{
    QPixmap pixmap = ui->labelQrCode->pixmap();
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Erreur", "Générez un QR code d'abord !");
        return;
    }

    QString file = QFileDialog::getSaveFileName(this, "Sauvegarder QR Code",
                                                "QR_Client_" + ui->id_client->text() + ".png", "PNG (*.png)");
    if (file.isEmpty()) return;

    if (pixmap.save(file, "PNG")) {
        QMessageBox::information(this, "Succès", "QR Code sauvegardé !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la sauvegarde");
    }
}
void MainWindow::copyQR()
{
    QPixmap pixmap = ui->labelQrCode->pixmap();
    if (pixmap.isNull()) {
        QMessageBox::warning(this, "Erreur", "Générez un QR code d'abord !");
        return;
    }
    QApplication::clipboard()->setPixmap(pixmap);
    QMessageBox::information(this, "Succès", "QR Code copié dans le presse-papier !");
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSqlQueryModel>
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // validators pour éviter saisie invalide dès la frappe
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]{0,12}$"), this));   // id (max 12 chiffres)
    ui->lineEdit_4->setValidator(new QRegularExpressionValidator(QRegularExpression("^[0-9]{0,8}$"), this));    // téléphone (max 8 chiffres)

    // affichage initial (sans filtre)
    QSqlQueryModel *model = Ftmp.afficher();
    remplirTable(model);
    delete model;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::remplirTable(QSqlQueryModel *model)
{
    if (!model) return;
    ui->tableWidget->clear();
    int rows = model->rowCount();
    int cols = model->columnCount();
    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(cols);

    // mettre en-têtes
    QStringList headers;
    for (int c = 0; c < cols; ++c) {
        headers << model->headerData(c, Qt::Horizontal).toString();
    }
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QString val = model->data(model->index(i, j)).toString();
            ui->tableWidget->setItem(i, j, new QTableWidgetItem(val));
        }
    }
}

// ---------- Validation stricte ----------
bool MainWindow::controleSaisie()
{
    QString idText = ui->lineEdit->text().trimmed();
    QString nom = ui->lineEdit_5->text().trimmed();
    QString adresse = ui->lineEdit_2->text().trimmed();
    QString email = ui->lineEdit_3->text().trimmed();
    QString tel = ui->lineEdit_4->text().trimmed();
    QString specialite = ui->comboBox->currentText().trimmed();

    // champs non vides
    if (idText.isEmpty() || nom.isEmpty() || adresse.isEmpty() || email.isEmpty() || tel.isEmpty() || specialite.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Tous les champs sont obligatoires !");
        return false;
    }

    // ID : exactement 12 chiffres
    QRegularExpression regexID("^[0-9]{12}$");
    if (!regexID.match(idText).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID doit contenir exactement 12 chiffres.");
        return false;
    }

    // Nom société : lettres uniquement (on accepte espaces et caractères accentués), max 20
    QRegularExpression regexNom("^[A-Za-zÀ-ÖØ-öø-ÿ\\s]{1,20}$");
    if (!regexNom.match(nom).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom de la société doit contenir uniquement des lettres (max 20).");
        return false;
    }

    // Adresse : lettres + chiffres + espaces, max 50
    QRegularExpression regexAdresse("^[A-Za-z0-9À-ÖØ-öø-ÿ\\s]{1,50}$");
    if (!regexAdresse.match(adresse).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'adresse doit contenir lettres et/ou chiffres (max 50).");
        return false;
    }

    // Email : max 30, doit contenir @ et format de base
    if (email.length() > 30) {
        QMessageBox::warning(this, "Erreur de saisie", "L'adresse e-mail ne doit pas dépasser 30 caractères.");
        return false;
    }
    QRegularExpression regexEmail("^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    if (!regexEmail.match(email).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Adresse e-mail invalide (doit contenir un '@' et un domaine).");
        return false;
    }

    // Téléphone : exactement 8 chiffres
    QRegularExpression regexTel("^[0-9]{8}$");
    if (!regexTel.match(tel).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le numéro de téléphone doit contenir exactement 8 chiffres.");
        return false;
    }

    // Spécialité : sélection
    if (specialite.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez sélectionner une spécialité.");
        return false;
    }

    return true;
}

// ---------- Helpers pour tri et filtre ----------
QString MainWindow::currentOrderBy()
{
    QString col = ui->comboBox_3->currentText().trimmed();
    if (col == "id") col = "id";
    else if (col == "nom société") col = "nom_societe";
    else if (col == "spécialité") col = "specialite";
    else col = ""; // défaut

    QString order;
    if (!col.isEmpty()) {
        // radioButton -> croissant, radioButton_3 -> décroissant
        if (ui->radioButton->isChecked()) order = col + " ASC";
        else if (ui->radioButton_3->isChecked()) order = col + " DESC";
        else order = col + " ASC";
    }
    return order;
}

QString MainWindow::buildFilterFromSearch(const QString &term)
{
    if (term.trimmed().isEmpty()) return QString();
    // Rechercher dans id (to_char/CAST selon SGBD) ou nom_societe ou specialite
    // Ici on utilise lower(...) LIKE '%term%'
    QString t = term.trimmed().toLower();
    // NOTE: lower cast works in many DBs; if your DB requires different functions, adaptez.
    QString filter = QString("LOWER(nom_societe) LIKE '%%1%' OR LOWER(specialite) LIKE '%%1%' OR CAST(id AS VARCHAR(100)) LIKE '%%1%'").arg(t);
    return filter;
}

// ---------- CRUD slots ----------
void MainWindow::on_pushButton_clicked()
{
    // Ajouter
    if (!controleSaisie()) return;

    int id = ui->lineEdit->text().toInt();
    QString nom = ui->lineEdit_5->text().trimmed();
    QString adresse = ui->lineEdit_2->text().trimmed();
    QString email = ui->lineEdit_3->text().trimmed();
    QString tel = ui->lineEdit_4->text().trimmed();
    QString specialite = ui->comboBox->currentText().trimmed();

    Fournisseur F(id, nom, adresse, email, tel, specialite);
    if (F.ajouter()) {
        QMessageBox::information(this, "Ajout", "Fournisseur ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout !");
    }

    // Rafraîchir affichage avec tri courant
    QString order = currentOrderBy();
    QSqlQueryModel *model = Ftmp.afficher(QString(), order);
    remplirTable(model);
    delete model;
}

void MainWindow::on_pushButton_8_clicked()
{
    // Supprimer
    QString idText = ui->lineEdit->text().trimmed();
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Saisir l'ID à supprimer.");
        return;
    }
    int id = idText.toInt();
    if (Ftmp.supprimer(id)) {
        QMessageBox::information(this, "Suppression", "Fournisseur supprimé !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression !");
    }

    QString order = currentOrderBy();
    QSqlQueryModel *model = Ftmp.afficher(QString(), order);
    remplirTable(model);
    delete model;
}

void MainWindow::on_pushButton_2_clicked()
{
    // Modifier
    if (!controleSaisie()) return;

    int id = ui->lineEdit->text().toInt();
    QString nom = ui->lineEdit_5->text().trimmed();
    QString adresse = ui->lineEdit_2->text().trimmed();
    QString email = ui->lineEdit_3->text().trimmed();
    QString tel = ui->lineEdit_4->text().trimmed();
    QString specialite = ui->comboBox->currentText().trimmed();

    Fournisseur F(id, nom, adresse, email, tel, specialite);
    if (F.modifier()) {
        QMessageBox::information(this, "Modification", "Fournisseur modifié avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification !");
    }

    QString order = currentOrderBy();
    QSqlQueryModel *model = Ftmp.afficher(QString(), order);
    remplirTable(model);
    delete model;
}

void MainWindow::on_pushButton_3_clicked()
{
    // Annuler : clear fields
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->comboBox->setCurrentIndex(0);
}

// ---------- Recherche ----------
void MainWindow::on_pushButton_7_clicked()
{
    QString term = ui->lineEdit_9->text().trimmed();
    // utiliser la méthode chercher (fournisseur)
    QSqlQueryModel *model = Fournisseur::chercher(term);
    remplirTable(model);
    delete model;
}

// ---------- Tri ----------
void MainWindow::on_pushButton_10_clicked()
{
    QString order = currentOrderBy();
    QSqlQueryModel *model = Ftmp.afficher(QString(), order);
    remplirTable(model);
    delete model;
}

// ---------- Export PDF ----------
void MainWindow::on_pushButton_9_clicked()
{
    // Générer fichier HTML à partir du tableau puis enregistrer en PDF via QPrinter
    QString fileName = QFileDialog::getSaveFileName(this, "Exporter PDF", QString(), "*.pdf");
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) fileName += ".pdf";

    // Récupérer les données actuelles du modèle affiché dans tableWidget
    int rows = ui->tableWidget->rowCount();
    int cols = ui->tableWidget->columnCount();

    QString html;
    html += "<html><head><meta charset='utf-8'><style>"
            "table { border-collapse: collapse; width: 100%; }"
            "th, td { border: 1px solid #000; padding: 4px; }"
            "th { background: #eee; }"
            "</style></head><body>";
    html += "<h2>Liste des fournisseurs</h2>";
    html += "<table><tr>";
    // headers
    for (int c = 0; c < cols; ++c) {
        QString h = ui->tableWidget->horizontalHeaderItem(c) ? ui->tableWidget->horizontalHeaderItem(c)->text() : QString("Col%1").arg(c);
        html += "<th>" + h.toHtmlEscaped() + "</th>";
    }
    html += "</tr>";
    for (int r = 0; r < rows; ++r) {
        html += "<tr>";
        for (int c = 0; c < cols; ++c) {
            QTableWidgetItem *it = ui->tableWidget->item(r, c);
            QString cell = it ? it->text() : QString();
            html += "<td>" + cell.toHtmlEscaped() + "</td>";
        }
        html += "</tr>";
    }
    html += "</table></body></html>";

    QTextDocument doc;
    doc.setHtml(html);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    doc.print(&printer);

    QMessageBox::information(this, "Export PDF", "Export terminé : " + fileName);
}

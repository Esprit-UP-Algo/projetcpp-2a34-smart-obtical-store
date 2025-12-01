#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QPdfWriter>
#include <QPainter>
#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChartView>
#include <QDialog>
#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Load employees when window opens
    refreshEmployeeTable();

    // Optional: Run CRUD tests in debug mode
    // testAllCRUD();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ============================================
// REFRESH TABLE - Reload data from database
// ============================================
void MainWindow::refreshEmployeeTable()
{
    Employee emp;
    QSqlQueryModel* model = emp.afficher();
    displayInTableWidget(model);
}

// ============================================
// CLEAR INPUT FIELDS
// ============================================
void MainWindow::clearInputFields()
{
    // Clear all input fields
    ui->lineEdit_id->clear();
    ui->lineEdit_nom->clear();
    ui->lineEdit_prenom->clear();
    ui->lineEdit_age->clear();  // Changed from spinBox_age
    ui->lineEdit_telephone->clear();
    ui->comboBox_2->setCurrentIndex(0);  // Reset comboBox to first item
    ui->lineEdit_mail->clear();
    ui->lineEdit_password->clear();
}

// ============================================
// ADD EMPLOYEE BUTTON
// ============================================
void MainWindow::on_pushButton_ajouter_clicked()
{
    qDebug() << "=== Add Button Clicked ===";

    // Get data from input fields
    QString nom = ui->lineEdit_nom->text().trimmed();
    QString prenom = ui->lineEdit_prenom->text().trimmed();
    QString ageText = ui->lineEdit_age->text().trimmed();
    int age = ageText.toInt();
    QString telephoneText = ui->lineEdit_telephone->text().trimmed();
    int telephone = telephoneText.toInt();
    QString poste = ui->comboBox_2->currentText().trimmed();
    QString mail = ui->lineEdit_mail->text().trimmed();
    QString motDePasse = ui->lineEdit_password->text().trimmed();

    // Debug: Print all values
    qDebug() << "Nom:" << nom;
    qDebug() << "Prenom:" << prenom;
    qDebug() << "Age:" << age;
    qDebug() << "Telephone:" << telephone;
    qDebug() << "Poste:" << poste;
    qDebug() << "Mail:" << mail;

    // Validate inputs
    if (nom.isEmpty() || prenom.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Le nom et le prénom sont obligatoires!");
        return;
    }

    if (ageText.isEmpty() || age <= 0 || age > 120)
    {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un âge valide (entre 1 et 120)!");
        return;
    }

    if (ui->comboBox_2->currentIndex() == -1 || poste.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un poste!");
        return;
    }

    if (mail.isEmpty() || !mail.contains("@"))
    {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une adresse email valide!");
        return;
    }

    if (telephoneText.isEmpty() || telephone <= 0)
    {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un numéro de téléphone valide!");
        return;
    }

    // Create employee object
    qDebug() << "Creating employee object...";
    Employee emp(nom, prenom, age, telephone, poste, mail, motDePasse);

    // Add to database
    qDebug() << "Attempting to add to database...";
    if (emp.ajouter())
    {
        qDebug() << "✓ Employee added successfully!";
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès!");
        refreshEmployeeTable();  // ⭐ THIS REFRESHES THE TABLE
        clearInputFields();
    }
    else
    {
        qDebug() << "✗ Failed to add employee!";
        QMessageBox::warning(this, "Erreur", "Erreur lors de l'ajout de l'employé! Vérifiez la console pour plus de détails.");
    }
}

// ============================================
// UPDATE EMPLOYEE BUTTON
// ============================================
void MainWindow::on_pushButton_modifier_clicked()
{
    // Get ID from input field
    int id = ui->lineEdit_id->text().toInt();

    if (id <= 0)
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un employé à modifier!");
        return;
    }

    // Get updated data from input fields
    QString nom = ui->lineEdit_nom->text().trimmed();
    QString prenom = ui->lineEdit_prenom->text().trimmed();
    int age = ui->lineEdit_age->text().toInt();  // Changed from spinBox_age
    int telephone = ui->lineEdit_telephone->text().toInt();
    QString poste = ui->comboBox_2->currentText().trimmed();  // Changed to comboBox_2
    QString mail = ui->lineEdit_mail->text().trimmed();
    QString motDePasse = ui->lineEdit_password->text().trimmed();

    // Validate inputs
    if (nom.isEmpty() || prenom.isEmpty() || mail.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Tous les champs obligatoires doivent être remplis!");
        return;
    }

    // Create employee object with ID
    Employee emp(id, nom, prenom, age, telephone, poste, mail, motDePasse);

    // Confirm modification
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment modifier cet employé?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        if (emp.modifier())
        {
            QMessageBox::information(this, "Succès", "Employé modifié avec succès!");
            refreshEmployeeTable();  // ⭐ THIS REFRESHES THE TABLE
            clearInputFields();
        }
        else
        {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la modification!");
        }
    }
}

// ============================================
// DELETE EMPLOYEE BUTTON
// ============================================
void MainWindow::on_pushButton_supprimer_clicked()
{
    qDebug() << "=== Delete Button Clicked ===";

    // Get selected row from table
    int selectedRow = ui->tableWidget->currentRow();
    qDebug() << "Selected row:" << selectedRow;

    if (selectedRow < 0)
    {
        qDebug() << "✗ No row selected!";
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un employé dans le tableau!");
        return;
    }

    // Get ID from first column of selected row
    QTableWidgetItem* idItem = ui->tableWidget->item(selectedRow, 0);
    if (!idItem)
    {
        qDebug() << "✗ Cannot read ID from table!";
        QMessageBox::warning(this, "Erreur", "Erreur lors de la lecture de l'ID!");
        return;
    }

    int id = idItem->text().toInt();
    qDebug() << "Employee ID to delete:" << id;

    if (id <= 0)
    {
        qDebug() << "✗ Invalid ID!";
        QMessageBox::warning(this, "Erreur", "ID invalide!");
        return;
    }

    // Get employee name for confirmation message
    QString nom = ui->tableWidget->item(selectedRow, 1)->text();
    QString prenom = ui->tableWidget->item(selectedRow, 2)->text();

    qDebug() << "Showing confirmation dialog for:" << nom << prenom;

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  QString("Voulez-vous vraiment supprimer %1 %2?\nCette action est irréversible!")
                                      .arg(nom, prenom),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qDebug() << "User confirmed deletion, attempting to delete ID:" << id;
        Employee emp;
        if (emp.supprimer(id))
        {
            qDebug() << "✓ Employee deleted successfully!";
            QMessageBox::information(this, "Succès", "Employé supprimé avec succès!");
            refreshEmployeeTable();  // ⭐ THIS REFRESHES THE TABLE
            clearInputFields();
        }
        else
        {
            qDebug() << "✗ Failed to delete employee!";
            QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression!");
        }
    }
    else
    {
        qDebug() << "User cancelled deletion";
    }
}


// ============================================
// SEARCH EMPLOYEE BUTTON
// ============================================
void MainWindow::on_pushButton_rechercher_clicked()
{
    qDebug() << "=== Search Button Clicked ===";

    QString searchTerm = ui->lineEdit_recherche->text().trimmed();
    qDebug() << "Search term:" << searchTerm;

    if (searchTerm.isEmpty())
    {
        QMessageBox::information(this, "Info", "Veuillez entrer un terme de recherche!");
        refreshEmployeeTable(); // Show all if empty
        return;
    }

    Employee emp;
    qDebug() << "Calling rechercher function...";
    QSqlQueryModel* model = emp.rechercher(searchTerm);
    qDebug() << "Search returned" << model->rowCount() << "results";

    // Display in tableWidget
    displayInTableWidget(model);

    // Show popup with result count
    if (model->rowCount() == 0)
    {
        QMessageBox::information(this, "Résultat", "Aucun employé trouvé!");
    }
    else
    {
        QString message = QString("%1 employé(s) trouvé(s) pour '%2'")
                              .arg(model->rowCount())
                              .arg(searchTerm);
        QMessageBox::information(this, "Résultat", message);

        qDebug() << "✓ Search successful:" << model->rowCount() << "results displayed";
    }
}

// ============================================
// SHOW ALL EMPLOYEES BUTTON
// ============================================
void MainWindow::on_pushButton_afficher_clicked()
{
    refreshEmployeeTable();
}

// ============================================
// CLEAR FORM BUTTON
// ============================================
void MainWindow::on_pushButton_clear_clicked()
{
    clearInputFields();
}

// ============================================
// TABLE CELL CLICKED - Load data to form
// ============================================
void MainWindow::on_tableWidget_cellClicked(int row, int column)
{
    // Get ID from first column of selected row
    QTableWidgetItem* idItem = ui->tableWidget->item(row, 0);
    if (idItem)
    {
        int id = idItem->text().toInt();
        loadEmployeeToForm(id);
    }
}

// ============================================
// LOAD EMPLOYEE DATA TO FORM
// ============================================
void MainWindow::loadEmployeeToForm(int id)
{
    Employee emp;
    Employee loadedEmp = emp.getEmployeeById(id);

    if (loadedEmp.getId() == id)
    {
        ui->lineEdit_id->setText(QString::number(loadedEmp.getId()));
        ui->lineEdit_nom->setText(loadedEmp.getNomEmploye());
        ui->lineEdit_prenom->setText(loadedEmp.getPrenomEmploye());
        ui->lineEdit_age->setText(QString::number(loadedEmp.getAge()));  // Changed from spinBox_age
        ui->lineEdit_telephone->setText(QString::number(loadedEmp.getTelephone()));

        // Set comboBox value - find and set the matching item
        QString poste = loadedEmp.getPoste();
        int index = ui->comboBox_2->findText(poste);
        if (index != -1)
        {
            ui->comboBox_2->setCurrentIndex(index);
        }

        ui->lineEdit_mail->setText(loadedEmp.getMail());
        ui->lineEdit_password->setText(loadedEmp.getMotDePasse());
    }
}

// ============================================
// DISPLAY DATA IN TABLE WIDGET
// ============================================
void MainWindow::displayInTableWidget(QSqlQueryModel* model)
{
    if (!ui->tableWidget)
    {
        qDebug() << "TableWidget not found!";
        return;
    }

    // Set up table
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(model->rowCount());
    ui->tableWidget->setColumnCount(model->columnCount());

    // Set headers
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "Age" << "Téléphone" << "Poste" << "Mail" << "Mot de Passe";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Populate data
    for (int row = 0; row < model->rowCount(); row++)
    {
        for (int col = 0; col < model->columnCount(); col++)
        {
            QTableWidgetItem* item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            ui->tableWidget->setItem(row, col, item);
        }
    }

    // Adjust column widths
    ui->tableWidget->resizeColumnsToContents();

    // Enable row selection
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    qDebug() << "✓ Data displayed in TableWidget (" << model->rowCount() << "rows)";
}

// ============================================
// TEST CRUD FUNCTIONS (Keep for debugging)
// ============================================





// ============================================
// SORT EMPLOYEES BUTTON
// ============================================
void MainWindow::on_pushButton_trier_clicked()
{
    qDebug() << "=== Sort Button Clicked ===";

    // Get the selected column from comboBox
    QString selectedColumn = ui->comboBox_tri->currentText(); // Changez "comboBox_tri" au nom exact de votre comboBox

    if (selectedColumn.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un critère de tri!");
        return;
    }

    // Map the display text to actual column names in database
    QString colonne;
    if (selectedColumn == "ID" || selectedColumn == "id")
    {
        colonne = "ID";
    }
    else if (selectedColumn == "Nom" || selectedColumn == "nom")
    {
        colonne = "NOM_EMPLOYE";
    }
    else if (selectedColumn == "Age" || selectedColumn == "age" || selectedColumn == "Âge")
    {
        colonne = "AGE";
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Critère de tri invalide!");
        return;
    }

    // Get the sort order from radio buttons
    QString ordre;
    if (ui->radioButton->isChecked()) // radioButton = Croissant
    {
        ordre = "ASC";
        qDebug() << "Sort order: Ascending";
    }
    else if (ui->radioButton_2->isChecked()) // radioButton_2 = Décroissant
    {
        ordre = "DESC";
        qDebug() << "Sort order: Descending";
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un ordre de tri (croissant ou décroissant)!");
        return;
    }

    qDebug() << "Sorting by:" << colonne << "Order:" << ordre;

    // Call the sort function
    Employee emp;
    QSqlQueryModel* model = emp.trierPar(colonne, ordre);

    // Display sorted results in tableWidget
    displayInTableWidget(model);

    // Show confirmation message
    QString orderText = (ordre == "ASC") ? "croissant" : "décroissant";
    QString message = QString("Employés triés par %1 en ordre %2")
                          .arg(selectedColumn)
                          .arg(orderText);

    QMessageBox::information(this, "Tri effectué", message);

    qDebug() << "✓ Sorting completed successfully";
}


// ============================================
// GENERATE PDF BUTTON
// ============================================
void MainWindow::on_PDF_clicked()
{
    qDebug() << "=== PDF Generation Started ===";

    // Get all employees
    Employee emp;
    QSqlQueryModel* model = emp.afficher();

    if (model->rowCount() == 0)
    {
        QMessageBox::warning(this, "Erreur", "Aucun employé à exporter!");
        return;
    }

    // Create file path with timestamp
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss");
    QString filePath = "C:/Users/user/Desktop/employe/liste_employes_" + timestamp + ".pdf";

    // Create QPdfWriter
    QPdfWriter pdfWriter(filePath);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageMargins(QMarginsF(15, 15, 15, 15));

    // Create QPainter to draw on PDF
    QPainter painter(&pdfWriter);

    // Set up fonts
    QFont titleFont("Arial", 20, QFont::Bold);
    QFont headerFont("Arial", 12, QFont::Bold);
    QFont normalFont("Arial", 10);

    int yPos = 500; // Starting Y position

    // Draw title
    painter.setFont(titleFont);
    painter.drawText(1000, yPos, "Liste des Employés");
    yPos += 800;

    // Draw date
    painter.setFont(normalFont);
    QString dateStr = "Date: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
    painter.drawText(1000, yPos, dateStr);
    yPos += 600;

    // Draw total count
    QString totalStr = QString("Total: %1 employé(s)").arg(model->rowCount());
    painter.drawText(1000, yPos, totalStr);
    yPos += 800;

    // Draw separator line
    painter.drawLine(500, yPos, 8000, yPos);
    yPos += 400;

    // Draw each employee
    for (int i = 0; i < model->rowCount(); i++)
    {
        // Check if we need a new page
        if (yPos > 11000) // Near bottom of page
        {
            pdfWriter.newPage();
            yPos = 500;
        }

        QString id = model->data(model->index(i, 0)).toString();
        QString nom = model->data(model->index(i, 1)).toString();
        QString prenom = model->data(model->index(i, 2)).toString();
        QString age = model->data(model->index(i, 3)).toString();
        QString telephone = model->data(model->index(i, 4)).toString();
        QString poste = model->data(model->index(i, 5)).toString();
        QString mail = model->data(model->index(i, 6)).toString();

        // Draw employee header (bold)
        painter.setFont(headerFont);
        QString employeeHeader = QString("Employé #%1 - %2 %3").arg(id, nom, prenom);
        painter.drawText(500, yPos, employeeHeader);
        yPos += 500;

        // Draw employee details
        painter.setFont(normalFont);
        painter.drawText(700, yPos, QString("Age: %1 ans").arg(age));
        yPos += 400;

        painter.drawText(700, yPos, QString("Téléphone: %1").arg(telephone));
        yPos += 400;

        painter.drawText(700, yPos, QString("Poste: %1").arg(poste));
        yPos += 400;

        painter.drawText(700, yPos, QString("Email: %1").arg(mail));
        yPos += 600;

        // Draw separator
        painter.setPen(QPen(Qt::lightGray, 2));
        painter.drawLine(500, yPos, 8000, yPos);
        painter.setPen(QPen(Qt::black, 1));
        yPos += 400;
    }

    // Finish painting
    painter.end();

    qDebug() << "✓ PDF generated successfully at:" << filePath;

    // Show success message with option to open
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Succès",
                                  QString("PDF généré avec succès!\n\n%1\n\nVoulez-vous ouvrir le fichier?")
                                      .arg(filePath),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        // Open the PDF file with default application
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
    }
}

void MainWindow::on_stat_clicked()
{
    qDebug() << "=== Statistics Button Clicked ===";

    // Get statistics from database
    Employee emp;
    QMap<QString, int> stats = emp.getStatistiquesParPoste();

    if (stats.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Aucune donnée statistique disponible!");
        return;
    }

    // Create pie series
    QPieSeries *series = new QPieSeries();

    // Define colors for the 3 postes
    QColor colors[3] = {
        QColor(255, 99, 132),   // Rose/Rouge
        QColor(54, 162, 235),   // Bleu
        QColor(255, 206, 86)    // Jaune
    };

    int colorIndex = 0;
    int totalEmployees = 0;

    // Add slices for each poste
    QMapIterator<QString, int> i(stats);
    while (i.hasNext())
    {
        i.next();
        QString poste = i.key();
        int count = i.value();
        totalEmployees += count;

        // Add slice
        QPieSlice *slice = series->append(poste, count);

        // Set color
        if (colorIndex < 3)
        {
            slice->setColor(colors[colorIndex]);
            colorIndex++;
        }

        // Make slice stand out on hover
        slice->setExplodeDistanceFactor(0.05);
        slice->setLabelVisible(true);
    }

    // Update labels with percentages
    for (QPieSlice *slice : series->slices())
    {
        slice->setLabel(QString("%1: %2 employé(s) (%3%)")
                            .arg(slice->label())
                            .arg((int)slice->value())
                            .arg(100.0 * slice->percentage(), 0, 'f', 1));
    }

    // Create chart
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QString("Statistiques des Employés par Poste\nTotal: %1 employé(s)").arg(totalEmployees));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    // Create chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Create dialog window
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Statistiques des Employés");
    dialog->resize(800, 600);

    // Set layout
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    layout->addWidget(chartView);
    dialog->setLayout(layout);

    // Show dialog
    dialog->exec();

    qDebug() << "✓ Statistics displayed successfully";
}
void MainWindow::on_pushButton_messagerie_clicked()
{
    MessagerieDialog *msgDialog = new MessagerieDialog(idEmployeConnecte, this);
    msgDialog->exec();
    delete msgDialog;
}


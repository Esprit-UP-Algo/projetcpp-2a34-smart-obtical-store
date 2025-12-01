#include "messagerie.h"
#include "ui_messagerie.h"
#include "employee.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MessagerieDialog::MessagerieDialog(int idEmployeConnecte, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MessagerieDialog)
    , idEmployeConnecte(idEmployeConnecte)
{
    ui->setupUi(this);

    qDebug() << "=== Messagerie opened for employee ID:" << idEmployeConnecte;

    // Load destinataires into combobox
    chargerDestinataires();

    // Load messages
    rafraichirMessagesRecus();
    rafraichirMessagesEnvoyes();

    // Show unread count
    afficherNombreNonLus();

    // Set default tab to inbox
    ui->tabWidget->setCurrentIndex(0);
}

MessagerieDialog::~MessagerieDialog()
{
    delete ui;
}

// ============================================
// CHARGER LES DESTINATAIRES DANS LE COMBOBOX
// ============================================
void MessagerieDialog::chargerDestinataires()
{
    qDebug() << "Loading destinataires...";

    ui->comboBox_destinataire->clear();
    ui->comboBox_destinataire->addItem("-- Sélectionnez un destinataire --", 0);

    QSqlQuery query;
    query.prepare("SELECT ID, NOM_EMPLOYE, PRENOM_EMPLOYE FROM EMPLOYÉ WHERE ID != :myId ORDER BY NOM_EMPLOYE");
    query.bindValue(":myId", idEmployeConnecte);

    if (query.exec())
    {
        while (query.next())
        {
            int id = query.value(0).toInt();
            QString nom = query.value(1).toString();
            QString prenom = query.value(2).toString();
            QString fullName = nom + " " + prenom;

            ui->comboBox_destinataire->addItem(fullName, id);
        }

        qDebug() << "✓ Loaded" << ui->comboBox_destinataire->count() - 1 << "employees";
    }
    else
    {
        qDebug() << "✗ Error loading destinataires:" << query.lastError().text();
    }
}

// ============================================
// RAFRAÎCHIR MESSAGES REÇUS
// ============================================
void MessagerieDialog::rafraichirMessagesRecus()
{
    qDebug() << "Refreshing received messages...";

    QSqlQueryModel* model = Message::afficherMessagesRecus(idEmployeConnecte);
    ui->tableView_recus->setModel(model);

    // Hide ID column
    ui->tableView_recus->setColumnHidden(0, true);

    // Adjust column widths
    ui->tableView_recus->resizeColumnsToContents();
    ui->tableView_recus->horizontalHeader()->setStretchLastSection(true);

    qDebug() << "✓ Loaded" << model->rowCount() << "received messages";

    // Update unread count
    afficherNombreNonLus();
}

// ============================================
// RAFRAÎCHIR MESSAGES ENVOYÉS
// ============================================
void MessagerieDialog::rafraichirMessagesEnvoyes()
{
    qDebug() << "Refreshing sent messages...";

    QSqlQueryModel* model = Message::afficherMessagesEnvoyes(idEmployeConnecte);
    ui->tableView_envoyes->setModel(model);

    // Hide ID column
    ui->tableView_envoyes->setColumnHidden(0, true);

    // Adjust column widths
    ui->tableView_envoyes->resizeColumnsToContents();
    ui->tableView_envoyes->horizontalHeader()->setStretchLastSection(true);

    qDebug() << "✓ Loaded" << model->rowCount() << "sent messages";
}

// ============================================
// AFFICHER NOMBRE DE MESSAGES NON LUS
// ============================================
void MessagerieDialog::afficherNombreNonLus()
{
    int count = Message::compterMessagesNonLus(idEmployeConnecte);

    QString notification;
    if (count == 0)
    {
        notification = "✅ Aucun nouveau message";
        ui->label_notifications->setStyleSheet(
            "background-color: #27ae60; color: white; padding: 8px; "
            "border-radius: 5px; font-size: 14px; font-weight: bold;");
    }
    else
    {
        notification = QString("🔔 Vous avez %1 nouveau(x) message(s)").arg(count);
        ui->label_notifications->setStyleSheet(
            "background-color: #e74c3c; color: white; padding: 8px; "
            "border-radius: 5px; font-size: 14px; font-weight: bold;");
    }

    ui->label_notifications->setText(notification);
    qDebug() << "Unread messages:" << count;
}

// ============================================
// ENVOYER UN NOUVEAU MESSAGE
// ============================================
void MessagerieDialog::on_pushButton_envoyer_clicked()
{
    qDebug() << "=== Send Message Button Clicked ===";

    // Get destinataire
    int idDestinataire = ui->comboBox_destinataire->currentData().toInt();

    if (idDestinataire == 0)
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un destinataire!");
        return;
    }

    // Get sujet and contenu
    QString sujet = ui->lineEdit_sujet->text().trimmed();
    QString contenu = ui->textEdit_contenu->toPlainText().trimmed();

    // Validate
    if (contenu.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Le message ne peut pas être vide!");
        return;
    }

    if (sujet.isEmpty())
    {
        sujet = "(Sans sujet)";
    }

    // Create and send message
    Message msg(idEmployeConnecte, idDestinataire, sujet, contenu);

    if (msg.envoyer())
    {
        QMessageBox::information(this, "Succès",
                                 QString("Message envoyé à %1!")
                                     .arg(ui->comboBox_destinataire->currentText()));

        // Clear form
        ui->comboBox_destinataire->setCurrentIndex(0);
        ui->lineEdit_sujet->clear();
        ui->textEdit_contenu->clear();

        // Refresh sent messages
        rafraichirMessagesEnvoyes();

        // Switch to sent tab
        ui->tabWidget->setCurrentIndex(1);
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'envoi du message!");
    }
}

// ============================================
// LIRE UN MESSAGE REÇU
// ============================================
void MessagerieDialog::on_pushButton_lire_clicked()
{
    qDebug() << "=== Read Message Button Clicked ===";

    // Get selected row
    QModelIndexList selection = ui->tableView_recus->selectionModel()->selectedRows();

    if (selection.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un message!");
        return;
    }

    // Get message ID from hidden column
    int row = selection.at(0).row();
    int idMessage = ui->tableView_recus->model()->data(
                                                    ui->tableView_recus->model()->index(row, 0)).toInt();

    qDebug() << "Reading message ID:" << idMessage;

    // Get message details
    Message msg = Message::getMessageById(idMessage);

    // Mark as read
    Message::marquerCommeLu(idMessage);

    // Get sender name
    QString expediteur = ui->tableView_recus->model()->data(
                                                         ui->tableView_recus->model()->index(row, 1)).toString();

    // Show message in dialog
    QString messageText = QString(
                              "━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
                              "De: %1\n"
                              "Sujet: %2\n"
                              "Date: %3\n"
                              "━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"
                              "%4"
                              ).arg(expediteur,
                                   msg.getSujet(),
                                   msg.getDateEnvoi().toString("dd/MM/yyyy HH:mm"),
                                   msg.getContenu());

    QMessageBox msgBox;
    msgBox.setWindowTitle("📨 Message");
    msgBox.setText(messageText);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();

    // Refresh to update read status
    rafraichirMessagesRecus();
}

// ============================================
// VOIR UN MESSAGE ENVOYÉ
// ============================================
void MessagerieDialog::on_pushButton_rafraichir_envoyes_clicked()
{
    qDebug() << "=== View Sent Message Button Clicked ===";

    // Get selected row
    QModelIndexList selection = ui->tableView_envoyes->selectionModel()->selectedRows();

    if (selection.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un message!");
        return;
    }

    // Get message ID
    int row = selection.at(0).row();
    int idMessage = ui->tableView_envoyes->model()->data(
                                                      ui->tableView_envoyes->model()->index(row, 0)).toInt();

    // Get message details
    Message msg = Message::getMessageById(idMessage);

    // Get recipient name
    QString destinataire = ui->tableView_envoyes->model()->data(
                                                             ui->tableView_envoyes->model()->index(row, 1)).toString();

    // Show message
    QString messageText = QString(
                              "━━━━━━━━━━━━━━━━━━━━━━━━━━\n"
                              "À: %1\n"
                              "Sujet: %2\n"
                              "Date: %3\n"
                              "━━━━━━━━━━━━━━━━━━━━━━━━━━\n\n"
                              "%4"
                              ).arg(destinataire,
                                   msg.getSujet(),
                                   msg.getDateEnvoi().toString("dd/MM/yyyy HH:mm"),
                                   msg.getContenu());

    QMessageBox msgBox;
    msgBox.setWindowTitle("📤 Message envoyé");
    msgBox.setText(messageText);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.exec();
}

// ============================================
// SUPPRIMER UN MESSAGE REÇU
// ============================================
void MessagerieDialog::on_pushButton_supprimer_recu_clicked()
{
    qDebug() << "=== Delete Message Button Clicked ===";

    // Get selected row
    QModelIndexList selection = ui->tableView_recus->selectionModel()->selectedRows();

    if (selection.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un message!");
        return;
    }

    // Confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer ce message?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        int row = selection.at(0).row();
        int idMessage = ui->tableView_recus->model()->data(
                                                        ui->tableView_recus->model()->index(row, 0)).toInt();

        if (Message::supprimerMessage(idMessage))
        {
            QMessageBox::information(this, "Succès", "Message supprimé avec succès!");
            rafraichirMessagesRecus();
        }
        else
        {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression!");
        }
    }
}

// ============================================
// RAFRAÎCHIR BUTTONS
// ============================================
void MessagerieDialog::on_pushButton_rafraichir_recus_clicked()
{
    rafraichirMessagesRecus();
    QMessageBox::information(this, "Rafraîchi", "Boîte de réception mise à jour!");
}


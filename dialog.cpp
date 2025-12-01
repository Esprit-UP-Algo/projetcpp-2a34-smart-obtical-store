#include "dialog.h"
#include "ui_dialog.h"
#include "employee.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include<QInputDialog>
#include<QRandomGenerator>


Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("Connexion - Gestion des Employés");

    // Set password field to hide text
    ui->lineEdit_password->setEchoMode(QLineEdit::Password);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_login_clicked()
{
    qDebug() << "=== Login Button Clicked ===";

    QString mail = ui->lineEdit_mail->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    // Validate inputs
    if (mail.isEmpty() || password.isEmpty())
    {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs!");
        return;
    }

    // Verify credentials
    if (verifierCredentials(mail, password))
    {
        qDebug() << "✓ Login successful for:" << mail;
        QMessageBox::information(this, "Succès", "Connexion réussie!");

        // Accept dialog and close
        accept();
    }
    else
    {
        qDebug() << "✗ Login failed for:" << mail;
        QMessageBox::warning(this, "Erreur", "Email ou mot de passe incorrect!");

        // Clear password field
        ui->lineEdit_password->clear();
        ui->lineEdit_password->setFocus();
    }
}

void Dialog::on_pushButton_annuler_clicked()
{
    qDebug() << "Login cancelled";
    reject(); // Close dialog and return Rejected
}

bool Dialog::verifierCredentials(QString mail, QString password)
{
    qDebug() << "Verifying credentials for:" << mail;

    QSqlQuery query;
    query.prepare("SELECT ID FROM EMPLOYÉ WHERE MAIL = :mail AND MOT_DE_PASSE = :password");
    query.bindValue(":mail", mail);
    query.bindValue(":password", password);

    if (query.exec())
    {
        if (query.next())
        {
            idEmployeConnecte = query.value(0).toInt();  // ← Stockez l'ID
            qDebug() << "✓ User found in database - ID:" << idEmployeConnecte;
            return true;
        }
        else
        {
            qDebug() << "✗ User not found or wrong password";
            return false;
        }
    }
    else
    {
        qDebug() << "✗ Query error:" << query.lastError().text();
        return false;
    }

}
QString generateVerificationCode()
{
    QString code;
    for (int i = 0; i < 6; i++)
    {
        code += QString::number(QRandomGenerator::global()->bounded(10));
    }
    return code;
}

void Dialog::on_pushButton_forgot_password_clicked()
{
    qDebug() << "=== Forgot Password Clicked ===";

    // Step 1: Ask for email
    bool ok;
    QString email = QInputDialog::getText(this, "Mot de passe oublié",
                                          "Entrez votre adresse email:",
                                          QLineEdit::Normal, "", &ok);

    if (!ok || email.isEmpty())
    {
        return;
    }

    // Step 2: Verify email exists in database
    QSqlQuery query;
    query.prepare("SELECT * FROM EMPLOYÉ WHERE MAIL = :mail");
    query.bindValue(":mail", email);

    if (!query.exec() || !query.next())
    {
        QMessageBox::warning(this, "Erreur", "Cette adresse email n'existe pas dans notre système!");
        return;
    }

    // Step 3: Generate verification code
    generatedCode = generateVerificationCode();
    codeGeneratedTime = QDateTime::currentDateTime();
    resetEmail = email;

    qDebug() << "Generated code:" << generatedCode << "for email:" << email;

    // Step 4: Send email with code
    EmailSender sender;

    QMessageBox::information(this, "Envoi en cours",
                             "Envoi du code de vérification en cours...\nVeuillez patienter.");

    if (sender.sendVerificationCode(email, generatedCode))
    {
        QMessageBox::information(this, "Succès",
                                 QString("Un code de vérification a été envoyé à:\n%1\n\n"
                                         "Le code est valide pendant 10 minutes.").arg(email));

        // Step 5: Ask user to enter the code
        askForVerificationCode();
    }
    else
    {
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors de l'envoi de l'email.\n"
                              "Vérifiez votre connexion internet et la configuration SMTP.");
    }
}

void Dialog::askForVerificationCode()
{
    bool ok;
    QString enteredCode = QInputDialog::getText(this, "Vérification",
                                                "Entrez le code à 6 chiffres reçu par email:",
                                                QLineEdit::Normal, "", &ok);

    if (!ok || enteredCode.isEmpty())
    {
        return;
    }

    // Check if code expired (10 minutes)
    qint64 elapsedMinutes = codeGeneratedTime.secsTo(QDateTime::currentDateTime()) / 60;

    if (elapsedMinutes > 10)
    {
        QMessageBox::warning(this, "Code expiré",
                             "Le code de vérification a expiré.\n"
                             "Veuillez demander un nouveau code.");
        return;
    }

    // Verify code
    if (enteredCode == generatedCode)
    {
        QMessageBox::information(this, "Succès", "Code vérifié avec succès!");
        askForNewPassword();
    }
    else
    {
        QMessageBox::warning(this, "Erreur", "Code incorrect!\nVeuillez réessayer.");
        askForVerificationCode(); // Ask again
    }
}

void Dialog::askForNewPassword()
{
    bool ok;
    QString newPassword = QInputDialog::getText(this, "Nouveau mot de passe",
                                                "Entrez votre nouveau mot de passe:",
                                                QLineEdit::Password, "", &ok);

    if (!ok || newPassword.isEmpty())
    {
        return;
    }

    if (newPassword.length() < 6)
    {
        QMessageBox::warning(this, "Erreur", "Le mot de passe doit contenir au moins 6 caractères!");
        askForNewPassword();
        return;
    }

    // Confirm password
    QString confirmPassword = QInputDialog::getText(this, "Confirmation",
                                                    "Confirmez votre nouveau mot de passe:",
                                                    QLineEdit::Password, "", &ok);

    if (!ok || confirmPassword.isEmpty())
    {
        return;
    }

    if (newPassword != confirmPassword)
    {
        QMessageBox::warning(this, "Erreur", "Les mots de passe ne correspondent pas!");
        askForNewPassword();
        return;
    }

    // Update password in database
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE EMPLOYÉ SET MOT_DE_PASSE = :password WHERE MAIL = :mail");
    updateQuery.bindValue(":password", newPassword);
    updateQuery.bindValue(":mail", resetEmail);

    if (updateQuery.exec())
    {
        QMessageBox::information(this, "Succès",
                                 "Votre mot de passe a été réinitialisé avec succès!\n"
                                 "Vous pouvez maintenant vous connecter avec votre nouveau mot de passe.");

        // Fill login form with the email
        ui->lineEdit_mail->setText(resetEmail);
        ui->lineEdit_password->setFocus();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la mise à jour du mot de passe!");
    }
}

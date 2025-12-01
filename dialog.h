#ifndef DIALOG_H
#define DIALOG_H
#include "emailsender.h"
#include <QDialog>
#include<QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();
    int getIdEmployeConnecte() const { return idEmployeConnecte; }

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_annuler_clicked();
    void on_pushButton_forgot_password_clicked();

private:
    Ui::Dialog *ui;
    int idEmployeConnecte;
    bool verifierCredentials(QString mail, QString password);
    QString generatedCode;
    QDateTime codeGeneratedTime;
    QString resetEmail;
    void askForVerificationCode();
    void askForNewPassword();
};

#endif // DIALOG_H

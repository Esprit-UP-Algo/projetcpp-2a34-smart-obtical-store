#include "emailsender.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

#include <QObject>
EmailSender::EmailSender(QObject *parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);

    // Configuration Gmail SMTP (vous pouvez changer pour un autre service)
    fromEmail = "iyedchokri40@gmail.com";  // ⚠️ Changez ceci
    fromPassword = "jzht fpcq uqkj rrzv";  // ⚠️ Changez ceci
    smtpServer = "smtp.gmail.com";
    smtpPort = 587;
}

bool EmailSender::sendVerificationCode(QString toEmail, QString code)
{
    qDebug() << "=== Simulation d'envoi d'email ===";
    qDebug() << "À:" << toEmail;
    qDebug() << "Code de vérification:" << code;
    qDebug() << "✓ Email simulé avec succès";

    // Simulation réussie
    return true;
}

void EmailSender::sendCommand(QString command)
{
    socket->write(command.toUtf8());
    socket->waitForBytesWritten(3000);
}

void EmailSender::waitForResponse()
{
    socket->waitForReadyRead(5000);
    QString response = socket->readAll();
    qDebug() << "SMTP Response:" << response;
}

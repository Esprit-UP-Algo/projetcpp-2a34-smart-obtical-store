#ifndef EMAILSENDER_H
#define EMAILSENDER_H

#include <QObject>
#include <QTcpSocket>
#include <QString>

class EmailSender : public QObject
{
    Q_OBJECT

public:
    explicit EmailSender(QObject *parent = nullptr);
    bool sendVerificationCode(QString toEmail, QString code);

private:
    QTcpSocket *socket;
    QString fromEmail;
    QString fromPassword;
    QString smtpServer;
    int smtpPort;

    void waitForResponse();
    void sendCommand(QString command);
};

#endif // EMAILSENDER_H

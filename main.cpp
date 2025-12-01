#include "mainwindow.h"
#include "dialog.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Establish database connection first
    Connection c;
    bool test = c.createconnect();

    if (test)
    {
        // Database connection successful
        qDebug() << "Database connected, showing login dialog...";

        // Show login dialog
        Dialog loginDialog;

        // If login is successful (Accepted), show main window
        if (loginDialog.exec() == QDialog::Accepted)
        {
            // Login successful, show main window
            MainWindow w;
            w.setIdEmployeConnecte(loginDialog.getIdEmployeConnecte());  // ← Passer l'ID
            w.show();
            return a.exec();
        }
        else
        {
            // User cancelled login or closed dialog
            qDebug() << "Login cancelled by user";
            return 0;
        }
    }
    else
    {
        // Database connection failed
        QMessageBox::critical(nullptr, QObject::tr("Database Error"),
                              QObject::tr("Impossible de se connecter à la base de données!\n"
                                          "Vérifiez votre configuration."),
                              QMessageBox::Ok);
        return -1;
    }
}

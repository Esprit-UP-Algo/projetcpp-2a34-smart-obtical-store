#ifndef MESSAGERIE


#define MESSAGERIE


#include <QDialog>
#include "message.h"

    QT_BEGIN_NAMESPACE
namespace Ui { class MessagerieDialog; }
QT_END_NAMESPACE

class MessagerieDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MessagerieDialog(int idEmployeConnecte, QWidget *parent = nullptr);
    ~MessagerieDialog();

private slots:
    void on_pushButton_envoyer_clicked();
    void on_pushButton_rafraichir_recus_clicked();
    void on_pushButton_rafraichir_envoyes_clicked();
    void on_pushButton_lire_clicked();
    void on_pushButton_supprimer_recu_clicked();

private:
    Ui::MessagerieDialog *ui;
    int idEmployeConnecte;

    void chargerDestinataires();
    void rafraichirMessagesRecus();
    void rafraichirMessagesEnvoyes();
    void afficherNombreNonLus();
};

#endif // MESSAGERIEDIAL


#ifndef QRCODEGENERATOR_H
#define QRCODEGENERATOR_H

#include <QPixmap>
#include <QString>

struct ClientQR {
    int id;
    QString nom;
    QString prenom;
    QString telephone;
    int points;

    // Constructeur par défaut
    ClientQR() : id(0), nom(""), prenom(""), telephone(""), points(0) {}

    // Constructeur avec paramètres
    ClientQR(int id, QString nom, QString prenom, QString telephone, int points)
        : id(id), nom(nom), prenom(prenom), telephone(telephone), points(points) {}
};

class QRCodeGenerator
{
public:
    static QPixmap generate(const ClientQR &c);
};

#endif

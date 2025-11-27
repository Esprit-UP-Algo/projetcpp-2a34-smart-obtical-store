#include "qrcodegenerator.h"
#include <QPainter>
#include <QFont>
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlError>

QPixmap QRCodeGenerator::generate(const ClientQR &c)
{
    // Mise à jour des points dans la base
    QSqlQuery q;
    q.prepare("UPDATE \"CLIENT\" SET \"POINTS_CLIENTS\" = \"POINTS_CLIENTS\" + 10 WHERE \"IDC\" = :id");
    q.bindValue(":id", c.id);
    if (!q.exec()) {
        QMessageBox::warning(nullptr, "Erreur", "Échec de la mise à jour des points: " + q.lastError().text());
    } else {
        QSqlQuery("COMMIT").exec();
    }

    // Créer une carte fidélité avec QR code simulé
    QPixmap card(400, 500);
    card.fill(QColor("#E8F5E9"));

    QPainter painter(&card);
    painter.setRenderHint(QPainter::Antialiasing);

    // Encadrement principal
    painter.setPen(QPen(QColor("#2E7D32"), 3));
    painter.setBrush(QColor("#FFFFFF"));
    painter.drawRoundedRect(10, 10, 380, 480, 15, 15);

    // En-tête avec fond coloré
    painter.setBrush(QColor("#4CAF50"));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(10, 10, 380, 60, 15, 15);

    // Titre
    painter.setPen(Qt::white);
    painter.setFont(QFont("Segoe UI", 18, QFont::Bold));
    painter.drawText(0, 10, 400, 60, Qt::AlignCenter, "💎 Carte Fidélité");

    // Zone QR code (carré avec pattern)
    int qrSize = 250;
    int qrX = (400 - qrSize) / 2;
    int qrY = 80;

    // Fond QR code
    painter.setBrush(Qt::white);
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(qrX, qrY, qrSize, qrSize);

    // Dessiner un pattern QR code simulé
    painter.setBrush(Qt::black);
    int moduleSize = qrSize / 15;

    // Coins caractéristiques des QR codes
    auto drawCornerPattern = [&](int startX, int startY) {
        // Carré externe
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (i == 0 || i == 6 || j == 0 || j == 6 ||
                    (i >= 2 && i <= 4 && j >= 2 && j <= 4)) {
                    painter.drawRect(startX + i * moduleSize,
                                     startY + j * moduleSize,
                                     moduleSize, moduleSize);
                }
            }
        }
    };

    // Dessiner les 3 coins
    drawCornerPattern(qrX, qrY); // Coin supérieur gauche
    drawCornerPattern(qrX + qrSize - 7 * moduleSize, qrY); // Coin supérieur droit
    drawCornerPattern(qrX, qrY + qrSize - 7 * moduleSize); // Coin inférieur gauche

    // Modules de données (pattern aléatoire mais structuré)
    for (int i = 8; i < 15; i++) {
        for (int j = 8; j < 15; j++) {
            // Éviter le coin inférieur droit
            if (i < 10 || j < 10) {
                if ((i * j) % 5 == 0 || (i + j) % 7 == 0) {
                    painter.drawRect(qrX + i * moduleSize,
                                     qrY + j * moduleSize,
                                     moduleSize, moduleSize);
                }
            }
        }
    }

    // Texte sous le QR code
    painter.setPen(QColor("#666666"));
    painter.setFont(QFont("Segoe UI", 10));
    painter.drawText(0, qrY + qrSize + 10, 400, 20, Qt::AlignCenter, "Scannez pour gagner des points");

    // Section informations client
    int infoY = qrY + qrSize + 40;

    painter.setPen(QColor("#2E7D32"));
    painter.setFont(QFont("Segoe UI", 16, QFont::Bold));
    painter.drawText(0, infoY, 400, 30, Qt::AlignCenter, c.prenom + " " + c.nom);

    painter.setFont(QFont("Segoe UI", 12));
    painter.setPen(Qt::darkGray);
    painter.drawText(0, infoY + 30, 400, 20, Qt::AlignCenter, "ID: " + QString::number(c.id));
    painter.drawText(0, infoY + 55, 400, 20, Qt::AlignCenter, "Téléphone: " + c.telephone);

    // Section points
    painter.setBrush(QColor("#FF9800"));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(100, infoY + 85, 200, 40, 20, 20);

    painter.setPen(Qt::white);
    painter.setFont(QFont("Segoe UI", 14, QFont::Bold));
    painter.drawText(0, infoY + 85, 400, 40, Qt::AlignCenter,
                     QString("⭐ %1 points").arg(c.points + 10));

    // Pied de page
    painter.setPen(QColor("#888888"));
    painter.setFont(QFont("Segoe UI", 8));
    painter.drawText(0, infoY + 130, 400, 20, Qt::AlignCenter, "GrandOptical - Votre vision, notre passion");

    QMessageBox::information(nullptr, "Succès",
                             QString("%1 %2 a gagné 10 points fidélité !\n\nNouveau total: %3 points")
                                 .arg(c.prenom, c.nom, QString::number(c.points + 10)));

    return card;
}

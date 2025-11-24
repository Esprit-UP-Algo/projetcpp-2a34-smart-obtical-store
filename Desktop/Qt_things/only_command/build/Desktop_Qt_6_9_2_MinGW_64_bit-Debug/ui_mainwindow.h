/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QWidget *widget_2;
    QLabel *label_14;
    QLabel *label_15;
    QLineEdit *ref;
    QRadioButton *decroissant_2;
    QComboBox *mod_;
    QLineEdit *m_tot;
    QPushButton *add;
    QPushButton *update;
    QPushButton *cancel;
    QLabel *label_16;
    QComboBox *tri_type;
    QRadioButton *decroissant;
    QLineEdit *recherche;
    QPushButton *recherchebutton;
    QTableWidget *tab_show_command;
    QFrame *line_3;
    QPushButton *supprimer;
    QFrame *line_2;
    QPushButton *imprimer;
    QPushButton *pushButton_10;
    QLabel *label_12;
    QLabel *label_13;
    QLineEdit *date;
    QComboBox *etat_payment;
    QLabel *label_17;
    QLineEdit *montant_a_payyer;
    QComboBox *recherche_type;
    QLabel *label_8;
    QLabel *label_10;
    QLabel *label_9;
    QLabel *label_11;
    QFrame *line;
    QLabel *label_60;
    QLineEdit *id_client;
    QWidget *tab_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1304, 633);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(20, 0, 1271, 1080));
        tab = new QWidget();
        tab->setObjectName("tab");
        widget_2 = new QWidget(tab);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(0, 0, 1221, 1080));
        widget_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 253, 240);"));
        label_14 = new QLabel(widget_2);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(40, 370, 181, 41));
        label_14->setStyleSheet(QString::fromUtf8("font: 900 italic 12pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);"));
        label_15 = new QLabel(widget_2);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(40, 420, 191, 41));
        label_15->setStyleSheet(QString::fromUtf8("font: 900 italic 12pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);"));
        ref = new QLineEdit(widget_2);
        ref->setObjectName("ref");
        ref->setGeometry(QRect(240, 170, 151, 41));
        ref->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        decroissant_2 = new QRadioButton(widget_2);
        decroissant_2->setObjectName("decroissant_2");
        decroissant_2->setGeometry(QRect(940, 190, 111, 25));
        decroissant_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        mod_ = new QComboBox(widget_2);
        mod_->addItem(QString());
        mod_->addItem(QString());
        mod_->addItem(QString());
        mod_->addItem(QString());
        mod_->setObjectName("mod_");
        mod_->setGeometry(QRect(240, 270, 151, 41));
        mod_->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        m_tot = new QLineEdit(widget_2);
        m_tot->setObjectName("m_tot");
        m_tot->setGeometry(QRect(240, 370, 151, 41));
        m_tot->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        add = new QPushButton(widget_2);
        add->setObjectName("add");
        add->setGeometry(QRect(410, 240, 90, 29));
        add->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);\n"
"color: rgb(0, 0, 0);"));
        update = new QPushButton(widget_2);
        update->setObjectName("update");
        update->setGeometry(QRect(410, 300, 90, 29));
        update->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);\n"
"color: rgb(0, 0, 0);"));
        cancel = new QPushButton(widget_2);
        cancel->setObjectName("cancel");
        cancel->setGeometry(QRect(410, 360, 90, 29));
        cancel->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);\n"
"color: rgb(0, 0, 0);"));
        label_16 = new QLabel(widget_2);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(830, 140, 131, 41));
        label_16->setStyleSheet(QString::fromUtf8("font: 900 italic 12pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);"));
        tri_type = new QComboBox(widget_2);
        tri_type->addItem(QString());
        tri_type->addItem(QString());
        tri_type->addItem(QString());
        tri_type->setObjectName("tri_type");
        tri_type->setGeometry(QRect(940, 140, 151, 41));
        tri_type->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(255, 170, 0);"));
        decroissant = new QRadioButton(widget_2);
        decroissant->setObjectName("decroissant");
        decroissant->setGeometry(QRect(940, 220, 111, 25));
        decroissant->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        recherche = new QLineEdit(widget_2);
        recherche->setObjectName("recherche");
        recherche->setGeometry(QRect(520, 200, 241, 41));
        recherche->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        recherchebutton = new QPushButton(widget_2);
        recherchebutton->setObjectName("recherchebutton");
        recherchebutton->setGeometry(QRect(830, 200, 101, 41));
        recherchebutton->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);\n"
"color: rgb(0, 0, 0);"));
        tab_show_command = new QTableWidget(widget_2);
        if (tab_show_command->columnCount() < 6)
            tab_show_command->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tab_show_command->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tab_show_command->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tab_show_command->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tab_show_command->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tab_show_command->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tab_show_command->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tab_show_command->setObjectName("tab_show_command");
        tab_show_command->setGeometry(QRect(510, 260, 691, 192));
        tab_show_command->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);\n"
"background-color: rgb(255, 170, 0);"));
        line_3 = new QFrame(widget_2);
        line_3->setObjectName("line_3");
        line_3->setGeometry(QRect(500, 110, 20, 401));
        line_3->setFrameShape(QFrame::Shape::VLine);
        line_3->setFrameShadow(QFrame::Shadow::Sunken);
        supprimer = new QPushButton(widget_2);
        supprimer->setObjectName("supprimer");
        supprimer->setGeometry(QRect(1100, 470, 90, 29));
        supprimer->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);\n"
"color: rgb(0, 0, 0);"));
        line_2 = new QFrame(widget_2);
        line_2->setObjectName("line_2");
        line_2->setGeometry(QRect(50, 520, 1231, 16));
        line_2->setFrameShape(QFrame::Shape::HLine);
        line_2->setFrameShadow(QFrame::Shadow::Sunken);
        imprimer = new QPushButton(widget_2);
        imprimer->setObjectName("imprimer");
        imprimer->setGeometry(QRect(880, 470, 90, 29));
        imprimer->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);\n"
"color: rgb(0, 0, 0);"));
        pushButton_10 = new QPushButton(widget_2);
        pushButton_10->setObjectName("pushButton_10");
        pushButton_10->setGeometry(QRect(1060, 230, 71, 21));
        pushButton_10->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 170, 0);\n"
"color: rgb(0, 0, 0);"));
        label_12 = new QLabel(widget_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(40, 320, 131, 41));
        label_12->setStyleSheet(QString::fromUtf8("font: 900 italic 12pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);"));
        label_13 = new QLabel(widget_2);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(40, 270, 181, 41));
        label_13->setStyleSheet(QString::fromUtf8("font: 900 italic 12pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);"));
        date = new QLineEdit(widget_2);
        date->setObjectName("date");
        date->setGeometry(QRect(240, 320, 151, 41));
        date->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        etat_payment = new QComboBox(widget_2);
        etat_payment->addItem(QString());
        etat_payment->addItem(QString());
        etat_payment->addItem(QString());
        etat_payment->setObjectName("etat_payment");
        etat_payment->setGeometry(QRect(240, 420, 151, 41));
        etat_payment->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_17 = new QLabel(widget_2);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(40, 470, 181, 41));
        label_17->setStyleSheet(QString::fromUtf8("font: 900 italic 12pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);"));
        montant_a_payyer = new QLineEdit(widget_2);
        montant_a_payyer->setObjectName("montant_a_payyer");
        montant_a_payyer->setGeometry(QRect(240, 470, 151, 41));
        montant_a_payyer->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        recherche_type = new QComboBox(widget_2);
        recherche_type->addItem(QString());
        recherche_type->addItem(QString());
        recherche_type->addItem(QString());
        recherche_type->setObjectName("recherche_type");
        recherche_type->setGeometry(QRect(760, 200, 71, 41));
        recherche_type->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        label_8 = new QLabel(widget_2);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(370, 10, 381, 41));
        label_8->setStyleSheet(QString::fromUtf8("font: 900 italic 16pt \"Segoe UI\";\n"
"color: rgb(255, 170, 0);"));
        label_10 = new QLabel(widget_2);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(520, 110, 281, 41));
        label_10->setStyleSheet(QString::fromUtf8("font: 900 italic 16pt \"Segoe UI\";\n"
"color: rgb(0, 0, 255);"));
        label_9 = new QLabel(widget_2);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(20, 120, 301, 41));
        label_9->setStyleSheet(QString::fromUtf8("font: 900 italic 16pt \"Segoe UI\";\n"
"color: rgb(0, 0, 255);"));
        label_11 = new QLabel(widget_2);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(40, 170, 131, 41));
        label_11->setStyleSheet(QString::fromUtf8("font: 900 italic 12pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);"));
        line = new QFrame(widget_2);
        line->setObjectName("line");
        line->setGeometry(QRect(30, 80, 1001, 16));
        line->setFrameShape(QFrame::Shape::HLine);
        line->setFrameShadow(QFrame::Shadow::Sunken);
        label_60 = new QLabel(widget_2);
        label_60->setObjectName("label_60");
        label_60->setGeometry(QRect(40, 220, 131, 41));
        label_60->setStyleSheet(QString::fromUtf8("font: 900 italic 12pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);"));
        id_client = new QLineEdit(widget_2);
        id_client->setObjectName("id_client");
        id_client->setGeometry(QRect(240, 220, 151, 41));
        id_client->setStyleSheet(QString::fromUtf8("color: rgb(0, 0, 0);"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1304, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "Montant total:", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Etat payment:", nullptr));
        decroissant_2->setText(QCoreApplication::translate("MainWindow", "croissant", nullptr));
        mod_->setItemText(0, QCoreApplication::translate("MainWindow", "visa", nullptr));
        mod_->setItemText(1, QCoreApplication::translate("MainWindow", "Mastercard", nullptr));
        mod_->setItemText(2, QCoreApplication::translate("MainWindow", "cache", nullptr));
        mod_->setItemText(3, QCoreApplication::translate("MainWindow", "cheque", nullptr));

        add->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        update->setText(QCoreApplication::translate("MainWindow", "Modifier", nullptr));
        cancel->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Trier par :", nullptr));
        tri_type->setItemText(0, QCoreApplication::translate("MainWindow", "Ref", nullptr));
        tri_type->setItemText(1, QCoreApplication::translate("MainWindow", "Date", nullptr));
        tri_type->setItemText(2, QCoreApplication::translate("MainWindow", "Total", nullptr));

        decroissant->setText(QCoreApplication::translate("MainWindow", "d\303\251croissant", nullptr));
        recherchebutton->setText(QCoreApplication::translate("MainWindow", "rechecher", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tab_show_command->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Ref", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tab_show_command->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Mode payment", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tab_show_command->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tab_show_command->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Total", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tab_show_command->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "etat", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tab_show_command->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Montan pey\303\251", nullptr));
        supprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer", nullptr));
        imprimer->setText(QCoreApplication::translate("MainWindow", "Exporter PDF", nullptr));
        pushButton_10->setText(QCoreApplication::translate("MainWindow", "trier", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Date:", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Mode payment:", nullptr));
        etat_payment->setItemText(0, QCoreApplication::translate("MainWindow", "payer", nullptr));
        etat_payment->setItemText(1, QCoreApplication::translate("MainWindow", "non payyer", nullptr));
        etat_payment->setItemText(2, QString());

        label_17->setText(QCoreApplication::translate("MainWindow", "Montant a payer:", nullptr));
        recherche_type->setItemText(0, QCoreApplication::translate("MainWindow", "Ref", nullptr));
        recherche_type->setItemText(1, QCoreApplication::translate("MainWindow", "Date", nullptr));
        recherche_type->setItemText(2, QCoreApplication::translate("MainWindow", "Total", nullptr));

        label_8->setText(QCoreApplication::translate("MainWindow", "Gestion Des Command", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Tableau command:", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Formulaire command:", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "R\303\251f\303\250rence :", nullptr));
        label_60->setText(QCoreApplication::translate("MainWindow", "ID Client :", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H

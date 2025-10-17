#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Link buttons to stacked widget pages
    connect(ui->acc, &QPushButton::clicked, this, [=](){
        ui->sw->setCurrentWidget(ui->RDV);   // or whichever page you want
    });

    connect(ui->client, &QPushButton::clicked, this, [=](){
        ui->sw->setCurrentWidget(ui->cli);
    });

    connect(ui->command_2, &QPushButton::clicked, this, [=](){
        ui->sw->setCurrentWidget(ui->command);
    });

    connect(ui->emp_2, &QPushButton::clicked, this, [=](){
        ui->sw->setCurrentWidget(ui->emp);
    });

    connect(ui->fournisseur, &QPushButton::clicked, this, [=](){
        ui->sw->setCurrentWidget(ui->fournisseurs);
    });

    connect(ui->prod_2, &QPushButton::clicked, this, [=](){
        ui->sw->setCurrentWidget(ui->prod);
    });

    connect(ui->rdv, &QPushButton::clicked, this, [=](){
        ui->sw->setCurrentWidget(ui->RDV);
    });
}
MainWindow::~MainWindow()
{
    delete ui;
}

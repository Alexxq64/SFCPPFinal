#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "chatdb.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushTemp_clicked()
{
    openDB();
    ui->tempLabel->setText(globalString);

}


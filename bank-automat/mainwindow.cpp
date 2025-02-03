#include "login.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

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

void MainWindow::on_btnStart_clicked()
{
    this->hide(); // Tämä piilottaa MainWindow ikkunan

    Login *objLogin = new Login(this);
    objLogin->setGeometry(this->geometry()); // Asetetaan Login ikkuna täsmälleen samaan kohtaan
    objLogin->show();

    // kun Login suljetaan, MainWindow näkyy taas
    connect(objLogin, &Login::finished, this, &MainWindow::show);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QApplication::quit(); // Sammuttaa koko sovelluksen
}

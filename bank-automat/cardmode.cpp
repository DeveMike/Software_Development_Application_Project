#include "cardmode.h"
#include "ui_cardmode.h"
#include "mainmenu.h"
#include <QApplication>

CardMode::CardMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CardMode)
{
    ui->setupUi(this);
}

CardMode::~CardMode()
{
    delete ui;
}
void CardMode::setUsername(const QString &username)
{
    mUsername = username;
}

void CardMode::setMyToken(const QString &token)
{
    mToken = token;
}

void CardMode::setLanguage(const QString &lang)
{
    mLanguage = lang;

    if (mLanguage == "ENG") {
        ui->btnDebit->setText("Debit");
        ui->btnCredit->setText("Credit");
    }
    else if (mLanguage == "SWE") {
        ui->btnDebit->setText("Debet");
        ui->btnCredit->setText("Kredit");
    }
    else {
        ui->btnDebit->setText("Debit");
        ui->btnCredit->setText("Credit");
    }
}

void CardMode::on_btnDebit_clicked()
{
    this->hide();
    MainMenu *objMainMenu = new MainMenu(this);
    objMainMenu->setGeometry(this->geometry());
    objMainMenu->setLanguage(mLanguage);
    objMainMenu->setMyToken(mToken.toUtf8());
    objMainMenu->setUsername(mUsername);
    objMainMenu->show();
    connect(objMainMenu, &MainMenu::finished, this, &CardMode::show);
}

void CardMode::on_btnCredit_clicked()
{
    this->hide();
    MainMenu *objMainMenu = new MainMenu(this);
    objMainMenu->setGeometry(this->geometry());
    objMainMenu->setLanguage(mLanguage);
    objMainMenu->setMyToken(mToken.toUtf8());
    objMainMenu->setUsername(mUsername);
    objMainMenu->show();
    connect(objMainMenu, &MainMenu::finished, this, &CardMode::show);
}

void CardMode::closeEvent(QCloseEvent *)
{
    QApplication::quit();
}

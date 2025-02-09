#include "cardmode.h"
#include "ui_cardmode.h"
#include "mainmenu.h"
#include <QApplication>

CardMode::CardMode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CardMode)
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");
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
        ui->txtDebit->setText("Debit");
        ui->txtCredit->setText("Credit");
    }
    else if (mLanguage == "SWE") {
        ui->txtDebit->setText("Debet");
        ui->txtCredit->setText("Kredit");
    }
    else {
        ui->txtDebit->setText("Debit");
        ui->txtCredit->setText("Credit");
    }
}

void CardMode::on_btnBack_clicked()
{
    this->hide();
    QWidget *parent = parentWidget();
    if (parent) {
        parent->show();
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
    objMainMenu->setCardMode("debit");
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
    objMainMenu->setCardMode("credit");
    objMainMenu->show();
    connect(objMainMenu, &MainMenu::finished, this, &CardMode::show);
}

void CardMode::closeEvent(QCloseEvent *)
{
    QApplication::quit();
}

#include "customerdata.h"
#include "mainmenu.h"
#include "ui_mainmenu.h"

MainMenu::MainMenu(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainMenu)
{
    ui->setupUi(this);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::setUsername(const QString &newUsername)
{
    idcard = newUsername;
    ui->labelUsername->setText("Asiakas: " + idcard);}

void MainMenu::setMyToken(const QByteArray &newMyToken)
{
    myToken = newMyToken;
    qDebug()<<"Main Menu";
    qDebug()<<myToken;
}

void MainMenu::on_btnData_clicked()
{
    CustomerData *objCustomerData=new CustomerData(this);
    objCustomerData->setIdcard(idcard);
    objCustomerData->setMyToken(myToken);
    objCustomerData->open();


}


/*void MainMenu::on_btnWithdraw_clicked()
{
    WithdrawWindow *withdraw = new WithdrawWindow(this);
    withdraw->setMyToken(myToken);
    withdraw->setIdcard(idcard);
    withdraw->show();
}


void MainMenu::on_btnBalance_clicked()
{
    BalanceWindow *balance = new BalanceWindow(this);
    balance->setMyToken(myToken);
    balance->setIdcard(idcard);
    balance->show()
}


void MainMenu::on_btnTransfer_clicked()
{
    TransferWindow *transfer = new TransferWindow(this);
    transfer->setMyToken(myToken);
    transfer->setIdcard(idcard);
    transfer->show();
}
*/


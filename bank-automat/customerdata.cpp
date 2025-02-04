#include "customerdata.h"
#include "environment.h"
#include "ui_customerdata.h"
#include <QDebug>

CustomerData::CustomerData(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerData)
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");
}

CustomerData::~CustomerData()
{
    delete ui;
}

void CustomerData::setMyToken(const QByteArray &newMyToken) {
    myToken = newMyToken;
}

void CustomerData::setIdcard(const QString &newIdcard) {
    idcard = newIdcard;
}

void CustomerData::setLanguage(const QString &newLanguage) {
    selectedLanguage = newLanguage;
    updateLanguage();
}

void CustomerData::updateLanguage()
{
    if (selectedLanguage == "FI") {
        ui->labelData->setText("Asiakastiedot:");
        ui->labelID->setText("Asiakas ID:");
        ui->labelData_3->setText("Etunimi:");
        ui->labelData_2->setText("Sukunimi:");
        ui->btnBack->setText("Takaisin");
    }
    else if (selectedLanguage == "SWE") {
        ui->labelData->setText("Kunduppgifter:");
        ui->labelID->setText("Kund ID:");
        ui->labelData_3->setText("Förnamn:");
        ui->labelData_2->setText("Efternamn:");
        ui->btnBack->setText("Tillbaka");
    }
    else if (selectedLanguage == "ENG") {
        ui->labelData->setText("Customer Data:");
        ui->labelID->setText("Customer ID:");
        ui->labelData_3->setText("First Name:");
        ui->labelData_2->setText("Last Name:");
        ui->btnBack->setText("Back");
    }
}

void CustomerData::showEvent(QShowEvent *event)
{
    QString site_url = Environment::base_url() + "/card/" + idcard;
    QNetworkRequest request(site_url);

    //WEBTOKEN START
    request.setRawHeader(QByteArray("Authorization"), myToken);
    //WEBTOKEN END
    dataManager = new QNetworkAccessManager(this);

    connect(dataManager, &QNetworkAccessManager::finished, this, &CustomerData::showDataSlot);

    reply = dataManager->get(request);

    QDialog::showEvent(event);
}

void CustomerData::showDataSlot(QNetworkReply *reply)
{
    response_data = reply->readAll();
    qDebug() << "Received data: " << response_data;

    QJsonDocument json_doc = QJsonDocument::fromJson(response_data);
    QJsonObject json_obj = json_doc.object();

    int customerId = json_obj["idcustomer"].toInt();
    ui->labelID->setText(QString::number(customerId));

    QString site_url = Environment::base_url() + "/customer/" + QString::number(customerId);
    QNetworkRequest request(site_url);
    request.setRawHeader(QByteArray("Authorization"), myToken);

    QNetworkAccessManager *customerDataManager = new QNetworkAccessManager(this);
    connect(customerDataManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *customerReply) {
        QByteArray customerData = customerReply->readAll();
        qDebug() << "Customer information: " << customerData;

        QJsonDocument customerJsonDoc = QJsonDocument::fromJson(customerData);
        QJsonObject customerJsonObj = customerJsonDoc.object();

        ui->labelFname->setText(customerJsonObj["fname"].toString());
        ui->labelLname->setText(customerJsonObj["lname"].toString());

        customerReply->deleteLater();
        customerDataManager->deleteLater();
    });

    customerDataManager->get(request);

    reply->deleteLater();
    dataManager->deleteLater();
}

void CustomerData::on_btnBack_clicked()
{
    this->hide();
    QWidget *mainMenu = parentWidget();

    if (mainMenu) {
        mainMenu->show();
    }
}

void CustomerData::closeEvent(QCloseEvent *)
{
    QApplication::quit();
}


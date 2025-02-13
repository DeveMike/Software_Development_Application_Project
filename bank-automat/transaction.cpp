#include "environment.h"
#include "transaction.h"
#include "ui_transaction.h"

TransactionWindow::TransactionWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionWindow),
    currentPage(0)
{
    ui->setupUi(this);
    this->setWindowTitle("ATM");

    connect(ui->btnNext, &QPushButton::clicked, this, &TransactionWindow::on_btnNext_clicked);
    connect(ui->btnPrev, &QPushButton::clicked, this, &TransactionWindow::on_btnPrev_clicked);

    // Set table columns
    ui->tableWidget->setColumnCount(5);
    QStringList headers = {"ID", "Type", "Description", "Amount", "Timestamp"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // Disable editing
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); // Select full row
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection); // Only one selection
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true); // Last column stretches
}

TransactionWindow::~TransactionWindow()
{
    delete ui;
}

void TransactionWindow::setLanguage(const QString &newLanguage)
{
    selectedLanguage = newLanguage;
    updateLanguage();
}

void TransactionWindow::updateLanguage()
{
    if (selectedLanguage == "FI") {
        ui->btnBack->setText("Takaisin");
        ui->btnNext->setText("Seuraava");
        ui->btnPrev->setText("Edellinen");
    }
    else if (selectedLanguage == "SWE") {
        ui->btnBack->setText("Tillbaka");
        ui->btnNext->setText("Nästa");
        ui->btnPrev->setText("Föregående");
    }
    else if (selectedLanguage == "ENG") {
        ui->btnBack->setText("Back");
        ui->btnNext->setText("Next");
        ui->btnPrev->setText("Previous");
    }
}

void TransactionWindow::fetchTransactions(const QString &idcard, const QByteArray &token)
{
    QString url = Environment::base_url() + "/transaction/account/" + idcard;
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", token);

    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &TransactionWindow::onTransactionDataReceived);
    networkManager->get(request);
}

void TransactionWindow::onTransactionDataReceived(QNetworkReply *reply)
{
    QByteArray responseData = reply->readAll();
    reply->deleteLater();
    networkManager->deleteLater();

    if (reply->error() == QNetworkReply::NoError)
    {
        qDebug() << "Transaction Data:" << responseData;

        transactionsList.clear();
        QJsonArray transactions = QJsonDocument::fromJson(responseData).array();

        for (const QJsonValue &value : transactions)
        {
            transactionsList.append(value.toObject());
        }

        currentPage = 0;
        updateTransactionList();
    }
    else
    {
        qDebug() << "Error fetching transactions:" << reply->errorString();
    }
}

void TransactionWindow::updateTransactionList()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    int startIndex = currentPage * itemsPerPage;
    int endIndex = qMin(startIndex + itemsPerPage, transactionsList.size());

    for (int i = startIndex; i < endIndex; ++i) {
        QJsonObject transaction = transactionsList[i];

        ui->tableWidget->insertRow(i - startIndex);

        // Jokaiselle solulle on asetettu fontti
        QFont cellFont;
        cellFont.setPointSize(11);

        ui->tableWidget->setItem(i - startIndex, 0, new QTableWidgetItem(QString::number(transaction["idtransaction"].toInt())));
        ui->tableWidget->item(i - startIndex, 0)->setFont(cellFont);

        ui->tableWidget->setItem(i - startIndex, 1, new QTableWidgetItem(transaction["type"].toString()));
        ui->tableWidget->item(i - startIndex, 1)->setFont(cellFont);

        ui->tableWidget->setItem(i - startIndex, 2, new QTableWidgetItem(transaction["description"].toString()));
        ui->tableWidget->item(i - startIndex, 2)->setFont(cellFont);

        ui->tableWidget->setItem(i - startIndex, 3, new QTableWidgetItem(transaction["amount"].toString()));
        ui->tableWidget->item(i - startIndex, 3)->setFont(cellFont);

        //Päivämäärä
        QString timestamp = transaction["timestamp"].toString();
        QDateTime dateTime = QDateTime::fromString(timestamp, Qt::ISODate);
        if (dateTime.isValid()) {
            timestamp = dateTime.toString("yyyy-MM-dd hh:mm:ss");
        } else {
            timestamp = "Invalid Date";
        }

        ui->tableWidget->setItem(i - startIndex, 4, new QTableWidgetItem(timestamp));
        ui->tableWidget->item(i - startIndex, 4)->setFont(cellFont);
    }

    ui->tableWidget->resizeColumnsToContents();

    ui->btnPrev->setEnabled(currentPage > 0);
    ui->btnNext->setEnabled(endIndex < transactionsList.size());
}

void TransactionWindow::on_btnNext_clicked()
{
    if ((currentPage + 1) * itemsPerPage < transactionsList.size()) {
        currentPage++;
        updateTransactionList();
    }
}

void TransactionWindow::on_btnPrev_clicked()
{
    if (currentPage > 0) {
        currentPage--;
        updateTransactionList();
    }
}

void TransactionWindow::on_btnBack_clicked()
{
    this->hide();
    QWidget *mainMenu = parentWidget();

    if (mainMenu) {
        mainMenu->show();
    }
}

void TransactionWindow::closeEvent(QCloseEvent *)
{
    QApplication::quit();
}

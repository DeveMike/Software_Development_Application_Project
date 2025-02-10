#ifndef TRANSACTIONWINDOW_H
#define TRANSACTIONWINDOW_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QCloseEvent>

namespace Ui {
class TransactionWindow;
}

class TransactionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TransactionWindow(QWidget *parent = nullptr);
    ~TransactionWindow();
    void setLanguage(const QString &newLanguage);
    void fetchTransactions(const QString &idcard, const QByteArray &token);

private slots:
    void onTransactionDataReceived(QNetworkReply *reply);
    void on_btnBack_clicked();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::TransactionWindow *ui;
    QString selectedLanguage;
    QNetworkAccessManager *networkManager;
    void updateLanguage();
};

#endif // TRANSACTIONWINDOW_H

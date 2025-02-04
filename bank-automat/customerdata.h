#ifndef CUSTOMERDATA_H
#define CUSTOMERDATA_H

#include <QDialog>
#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

namespace Ui {
class CustomerData;
}

class CustomerData : public QDialog
{
    Q_OBJECT

public:
    explicit CustomerData(QWidget *parent = nullptr);
    ~CustomerData();
    void setIdcard(const QString &newIdcard);
    void setMyToken(const QByteArray &newMyToken);
    void setLanguage(const QString &newLanguage);

private slots:
    void showDataSlot(QNetworkReply *reply);
    void on_btnBack_clicked();

protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *) override;

private:
    Ui::CustomerData *ui;
    QString idcard;
    QByteArray myToken;
    QString selectedLanguage;

    QNetworkAccessManager *dataManager;
    QNetworkReply *reply;
    QByteArray response_data;
    void updateLanguage();
};

#endif // CUSTOMERDATA_H

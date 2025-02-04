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

private slots:
    void showDataSlot(QNetworkReply *reply);

protected:
    void showEvent(QShowEvent *event);

private:
    Ui::CustomerData *ui;
    QString idcard;
    QByteArray myToken;

    QNetworkAccessManager *dataManager;
    QNetworkReply *reply;
    QByteArray response_data;
};

#endif // CUSTOMERDATA_H

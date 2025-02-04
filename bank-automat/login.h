#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_btnLogin_clicked();
    void LoginSlot (QNetworkReply *reply);
    void on_btnLangFI_clicked();
    void on_btnLangSWE_clicked();
    void on_btnLangENG_clicked();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    Ui::Login *ui;
    QNetworkAccessManager *postManager;
    QNetworkReply *reply;
    QByteArray response_data;
    QString selectedLanguage = "FI";
    void updateLanguage();
};

#endif // LOGIN_H

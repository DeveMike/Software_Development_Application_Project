#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>
#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QJsonDocument>

namespace Ui {
class MainMenu;
}

class MainMenu : public QDialog
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = nullptr);
    ~MainMenu();
    void setUsername(const QString &newUsername);
    void setMyToken(const QByteArray &newMyToken);
    QByteArray getMyToken() const;
    void setLanguage(const QString &newLanguage);
    void setCardMode(const QString &mode);

private slots:
    void on_btnData_clicked();
    void onCardDataReceived(QNetworkReply *reply);
    void on_btnWithdraw_clicked();
    void on_btnTransfer_clicked();
    void on_btnBalance_clicked();
    void on_btnTransaction_clicked();
    void on_btnLogout_clicked();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    Ui::MainMenu *ui;
    QString idcard;
    QByteArray myToken;
    QString getLanguage() const;
    QString selectedLanguage;
    QString mCardMode;
    QNetworkAccessManager *dataManager = nullptr;
};

#endif // MAINMENU_H

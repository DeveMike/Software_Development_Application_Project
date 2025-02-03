#ifndef MAINMENU_H
#define MAINMENU_H

#include <QDialog>

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

private slots:
    void on_btnData_clicked();

    // void on_btnWithdraw_clicked();

    // void on_btnBalance_clicked();

    // void on_btnTransfer_clicked();

private:
    Ui::MainMenu *ui;
    QString idcard;
    QByteArray myToken;

};

#endif // MAINMENU_H

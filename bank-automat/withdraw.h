#ifndef WITHDRAWWINDOW_H
#define WITHDRAWWINDOW_H

#include <QDialog>

namespace Ui {
class WithdrawWindow;
}

class WithdrawWindow : public QDialog
{
    Q_OBJECT

public:
    explicit WithdrawWindow(QWidget *parent = nullptr);
    ~WithdrawWindow();
    void setLanguage(const QString &newLanguage);

private slots:
    void on_btnBack_clicked();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    Ui::WithdrawWindow *ui;
    QString selectedLanguage;
    void updateLanguage();
};

#endif // WITHDRAWWINDOW_H

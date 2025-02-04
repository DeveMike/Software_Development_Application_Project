#ifndef TRANSFERWINDOW_H
#define TRANSFERWINDOW_H

#include <QDialog>

namespace Ui {
class TransferWindow;
}

class TransferWindow : public QDialog
{
    Q_OBJECT

public:
    explicit TransferWindow(QWidget *parent = nullptr);
    ~TransferWindow();
    void setLanguage(const QString &newLanguage);

private slots:
    void on_btnBack_clicked();

protected:
    void closeEvent(QCloseEvent *) override;

private:
    Ui::TransferWindow *ui;
    QString selectedLanguage;
    void updateLanguage();
};

#endif // TRANSFERWINDOW_H

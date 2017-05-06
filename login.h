#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QtSql>
#include <QMainWindow>

extern bool openConnection(QSqlDatabase db);

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_lineEdit_username_editingFinished();

    void on_pushButton_login_clicked();



private:
    Ui::Login *ui;

public:
    void closeEvent(QCloseEvent*);

};

#endif // LOGIN_H

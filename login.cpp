#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QCloseEvent>
#include "workerfunctions.h"

extern bool openConnection(QSqlDatabase db);

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    ui->lineEdit_username->setProperty("mandatoryField", true);

    ui->lineEdit_password->setProperty("mandatoryField", true);
    ui->status_label->hide();



}

Login::~Login()
{

    delete ui;
}

void Login::on_lineEdit_username_editingFinished()
{
    ui->pushButton_login->setEnabled(true);
}

void Login::on_pushButton_login_clicked()
{
    QString username = ui->lineEdit_username->text();
    QString password = ui->lineEdit_password->text();

    if(username.isEmpty() || password.isEmpty())
    {
        QMessageBox::critical(this, "Login error", "Please enter your password");
    }


    bool authenticate = WorkerFunctions::loginAuthenticate(username, password);
    if(authenticate)
    {
        this->hide();

    }else{

        ui->lineEdit_password->setStyleSheet("border:1px solid red");
        ui->lineEdit_password->clear();
        ui->status_label->show();
        ui->status_label->setStyleSheet("background-color:red;" "color:white;" "font-size:12px;");
        ui->status_label->setText("Invalid username or password");
    }
}

void Login::closeEvent(QCloseEvent* event)
{
    if(event->Close)
    {
        qApp->quit();
    }

    delete event;
}

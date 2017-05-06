#include "about.h"
#include "ui_about.h"
#include <QDir>
#include <QTextStream>
#include <QFile>


About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    setup_about();
}

About::~About()
{
    delete ui;
}

void About::on_Close_pushButton_clicked()
{
    this->close();
}

void About::setup_about()
{
    QPixmap logo(QDir::currentPath() + "/logo.jpg");
    ui->logo_label->setPixmap(logo);

    QString about_bma = get_abouttext();
    ui->about_plainTextEdit->setPlainText(about_bma);
    ui->about_plainTextEdit->setReadOnly(true);
}

QString About::get_abouttext()
{
    QFile about_file(QDir::currentPath() + "/about.txt");
    about_file.open(QIODevice::ReadOnly);
    QTextStream about_text(&about_file);

    QString about_bma = about_text.readAll();

    return about_bma;


}

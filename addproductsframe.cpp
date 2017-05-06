#include "addproductsframe.h"
#include "ui_addproductsframe.h"
#include <QMessageBox>
#include "mainwindow.h"
#include "workerfunctions.h"
#include <QDebug>

AddProductsFrame::AddProductsFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::AddProductsFrame)
{
    ui->setupUi(this);

    ui->Products_comboBox->hide();
    ui->Delete_pushButton->hide();
    ui->Update_pushButton->hide();
    ui->label->hide();

    QDoubleValidator* priceval = new QDoubleValidator(ui->Price_lineEdit);
    priceval->setDecimals(2);
    ui->Price_lineEdit->setValidator(priceval);

    QRegExpValidator* nameval = new QRegExpValidator(QRegExp("[0-9A-Za-z' '!@#$%&*()_-]{1,20}"),ui->name_lineEdit);
    ui->name_lineEdit->setValidator(nameval);
}

AddProductsFrame::~AddProductsFrame()
{
    delete ui;
}

void AddProductsFrame::on_Cancel_pushButton_clicked()
{
    QString name = ui->name_lineEdit->text();
    QString pric = ui->Price_lineEdit->text();

    if(!name.isEmpty() || !pric.isEmpty())
    {
        QMessageBox::StandardButton reply;

        reply = QMessageBox::critical(this, "Add products cancel", "Are you sure you want to cancel??", QMessageBox::Yes | QMessageBox::No);

        if(reply == QMessageBox::Yes)
        {
            ui->Price_lineEdit->clear();
            ui->name_lineEdit->clear();
        }
    }

}

//saves new product to the system.
void AddProductsFrame::on_Save_pushButton_clicked()
{
    QString name = ui->name_lineEdit->text();
    QString pric = ui->Price_lineEdit->text();

    if(name.isEmpty() || pric.isEmpty())
    {
        QMessageBox::critical(this, "Add product", "Name and Price required");
        return;
    }

    int numberofproducts = WorkerFunctions::getSavedCount("Products");
    if(numberofproducts <=9 ){
        bool saveProduct = WorkerFunctions::saveProduct(name.toUpper(), pric.toDouble());
        if(saveProduct)
        {
            QMessageBox::information(this, "Saved", "Product Saved");
            ui->name_lineEdit->clear();
            ui->Price_lineEdit->clear();
        }else
        {
            QMessageBox::warning(this, "Save Product", "Product already in the system");
        }
    }else{
        QMessageBox::information(this, "save product", "This is a trial version. You can not save more than 10 products. Please purchase the full version");
    }
}

void AddProductsFrame::on_Update_pushButton_clicked()
{
    QString newname = ui->name_lineEdit->text();
    QString newprice = ui->Price_lineEdit->text();
    int id = ui->label->text().toDouble();

    if(!newname.isEmpty() || !newprice.isEmpty())
    {

        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(this, "Update", "Are you sure you want to update '"+newname+"'", QMessageBox::Yes | QMessageBox::No);

        if(reply == QMessageBox::Yes)
        {
            WorkerFunctions::updateProduct(id, newprice.toInt());
            QMessageBox::information(this, "Edit Product", "Updated");
            ui->name_lineEdit->clear();
            ui->Price_lineEdit->clear();

        }
    }else
    {
        QMessageBox::critical(this, "Edit Product", "Name and Price required");
    }
}

void AddProductsFrame::on_Loadlist_pushButton_clicked()
{
    if(ui->Products_comboBox->isHidden())
    {
        WorkerFunctions::returnModel(&model);
        ui->Products_comboBox->setModel(&model);
        ui->name_lineEdit->setReadOnly(true);
        ui->Products_comboBox->show();
        ui->Update_pushButton->show();
        ui->Delete_pushButton->show();
        ui->Save_pushButton->hide();
        ui->Cancel_pushButton->hide();
    }
    else
    {
        ui->name_lineEdit->setReadOnly(false);
        ui->name_lineEdit->clear();
        ui->Price_lineEdit->clear();
        ui->Products_comboBox->hide();
        ui->Update_pushButton->hide();
        ui->Delete_pushButton->hide();
        ui->Save_pushButton->show();
        ui->Cancel_pushButton->show();

    }

}

void AddProductsFrame::on_Products_comboBox_currentIndexChanged(const QString &arg1)
{

    QStringList productdetails = WorkerFunctions::returnValues(arg1);
    ui->label->setText(productdetails.value(0));
    ui->name_lineEdit->setText(productdetails.value(1));
    ui->Price_lineEdit->setText(productdetails.value(2));
}

void AddProductsFrame::on_Delete_pushButton_clicked()
{

    int id = ui->label->text().toInt();
    QString productname = ui->name_lineEdit->text();
    QMessageBox::StandardButton reply;

    reply = QMessageBox::question(this, "Delete", "Are you sure you want to delete '"+productname+"'", QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes)
    {

        WorkerFunctions::deleteProduct(id);
        QMessageBox::information(this, "Delete", "Deleted");
        ui->name_lineEdit->clear();
        ui->Price_lineEdit->clear();
        ui->Products_comboBox->clear();
    }

}




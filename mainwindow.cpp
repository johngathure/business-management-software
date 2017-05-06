#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <login.h>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QTableView>
#include <QTableWidget>
#include <QComboBox>
#include <QCalendarWidget>
#include <QInputDialog>
#include "addproductsframe.h"
#include "workerfunctions.h"
#include "global_var.h"
#include <QSqlRelationalTableModel>
#include <tableformat.h>
#include <QResizeEvent>
#include <QTimer>
#include <QDialog>
#include <QDateTime>
#include "about.h"
#include <QHash>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    QPixmap background(QDir::currentPath() + "/app_background.jpg");
    background.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette bkgimage;
    bkgimage.setBrush(QPalette::Background, background);
    this->setPalette(bkgimage);

    ui->Sales_groupBox->hide();
    ui->Purchases_groupBox->hide();
    ui->Products_groupBox->hide();
    ui->Stock_groupBox->hide();
    QTimer::singleShot(4020, this, SLOT(authenticateUser()));

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
    timer->start();
}

//destractor
MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::showTime()
{
    QString format = "dddd,dd MMM yyyy hh:mm";
    QString datetime = QDateTime::currentDateTime().toString(format);
    ui->datetime_label->setText(datetime);
}

void MainWindow::authenticateUser()
{
    Login* authenticate = new Login(this);
    authenticate->setModal(true);
    authenticate->show();

}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    if(event->Resize)
    {
        extern int user_id;
        QString username = WorkerFunctions::getUser(user_id);
        ui->attendant_label->setText("ATTENDANT : " + username);
    }

    delete event;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////// UI MANIPULATOR FUNCTIONS /////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//control group boxes implementation
void MainWindow::controlGroupboxes(QGroupBox* groupbox)
{
    if(groupbox->isHidden())
    {
        groupbox->show();

    }else
    {
        groupbox->hide();
    }
}

//control layout implementation
void MainWindow::controlLayouts(QVBoxLayout* layout_one, QVBoxLayout* layout_two)
{
    if(!layout_one->isEmpty())
    {
        QLayoutItem* removewidget = NULL;

        while((removewidget = layout_one->takeAt(0)) != 0)
        {
            delete removewidget->widget();
        }
    }

    if(layout_two != 0 && !layout_two->isEmpty())
    {
        QLayoutItem* removewidget = NULL;

        while((removewidget = layout_two->takeAt(0)) != 0)
        {
            delete removewidget->widget();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////// UI SLOTS //////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//on sales commandlink button clicked
void MainWindow::on_Sales_commandLinkButton_clicked()
{
    controlGroupboxes(ui->Sales_groupBox);
}

//on purchases commandlink button clicked
void MainWindow::on_Purchases_commandLinkButton_clicked()
{
    controlGroupboxes(ui->Purchases_groupBox);
}

//on products commandlink button clicked
void MainWindow::on_Products_commandLinkButton_clicked()
{
    controlGroupboxes(ui->Products_groupBox);
}

//on stock commandlink button clicked
void MainWindow::on_Stock_commandLinkButton_clicked()
{
    controlGroupboxes(ui->Stock_groupBox);

}

//dispalys products in the sysyem.
void MainWindow::on_ViewProduct_pushButton_clicked()
{
    ui->Info_label->setText("Viewing products");
    controlLayouts(ui->workspacearea_VL, ui->controlarea_VL);

    setup_ViewStockAndProducts();


    WorkerFunctions::openConnection();
    tablemodel = new QSqlTableModel;
    tablemodel->setTable("Products");
    tablemodel->select();

    tablemodel->removeColumn(0);
    tablemodel->setHeaderData(1, Qt::Horizontal, tr("Price per Unit"));
    tablemodel->setHeaderData(0, Qt::Horizontal, tr("Product"));
    purchases = new QTableView;
    purchases->setModel(tablemodel);
    purchases->setEditTriggers(QAbstractItemView::NoEditTriggers);
    purchases->setAlternatingRowColors(true);
    purchases->setStyleSheet("background-color:transparent;");
    purchases->setItemDelegateForColumn(1,new TableFormat(purchases));

    for(int column=0; column<2; column++){
        purchases->setColumnWidth(column, 200);
    }

    ui->workspacearea_VL->addWidget(purchases);
    WorkerFunctions::closeConnection();

    connect(searchstockbtn, SIGNAL(clicked()), this, SLOT(returnsearchproduct()));

}

void MainWindow::returnsearchproduct()
{
    QString productname = searchstock->text().toUpper();

    if(productname.isEmpty())
        QMessageBox::warning(this, "Search product", "Please enter a product name");
    else
    {
        float price = WorkerFunctions::getProduct(productname);

        QLocale locale;

        if(price == 0.00)
        {
            QMessageBox::warning(this, "Search product", "Product not found");
        }else{
            QString outputmessage = QString("Product: %1 \n Price: %2").arg(productname).arg(locale.toCurrencyString(price, "Kshs "));
            QMessageBox::information(this, "Search product", outputmessage);
        }
    }
}

//sets up a form for adding a new product.
void MainWindow::on_NewProduct_pushButton_clicked()
{
    ui->Info_label->setText("New product");
    controlLayouts(ui->workspacearea_VL, ui->controlarea_VL);

    addproducts = new AddProductsFrame;
    ui->controlarea_VL->addWidget(addproducts);
}

//sets up new purchase and sale ui
void MainWindow::setup_NewPurchaseAndSale(QString option1, QString option2)
{
    controlLayouts(ui->workspacearea_VL, ui->controlarea_VL);
    if(option1 == "Add sale")
        ui->Info_label->setText("New sale");
    else if(option1 == "Add purchase")
        ui->Info_label->setText("New purchase");

    totalprice = new QLabel;
    totalprice->setText("Total price");
    totalprice->setWordWrap(true);
    totalprice->setObjectName("totalprice");

    showtotalprice = new QLabel;
    showtotalprice->setText("0.00");
    showtotalprice->setObjectName("showtotalprice");

    printbutton = new QPushButton;
    printbutton->setText("Print");
    printbutton->setMinimumHeight(40);

    addrow = new QPushButton;
    addrow->setText(option1);
    addrow->setMinimumWidth(60);
    addrow->setMaximumHeight(40);
    addrow->setMinimumHeight(40);

    removerow = new QPushButton;
    removerow->setText(option2);
    removerow->setMinimumWidth(60);
    removerow->setMaximumHeight(40);
    removerow->setMinimumHeight(40);

    purchaseframe = new QFrame(this);
    smalllayout = new QGridLayout;
    smalllayout->addWidget(totalprice, 0, 0);
    smalllayout->addWidget(showtotalprice, 0,1);
    smalllayout->addWidget(addrow,1,0);
    smalllayout->addWidget(removerow,1,1);
    smalllayout->addWidget(printbutton, 2,0);


    purchaseframe->setLayout(smalllayout);
    ui->controlarea_VL->addWidget(purchaseframe,10,Qt::AlignTop);

    newpurchase = new QTableWidget;
    newpurchase->setColumnCount(4);
    newpurchase->setHorizontalHeaderItem(0, new QTableWidgetItem("Product Name"));
    newpurchase->setHorizontalHeaderItem(1, new QTableWidgetItem("Price per unit"));
    newpurchase->setHorizontalHeaderItem(2, new QTableWidgetItem("Quantity"));
    newpurchase->setHorizontalHeaderItem(3, new QTableWidgetItem("Total price"));
    newpurchase->setRowCount(1);
    newpurchase->setAlternatingRowColors(true);
//    newpurchase->setItemDelegateForColumn(1, new TableFormat(newpurchase));
//    newpurchase->setItemDelegateForColumn(2, new TableFormat(newpurchase));
//    newpurchase->setItemDelegateForColumn(3, new TableFormat(newpurchase));

    ui->workspacearea_VL->addWidget(newpurchase);


    connect(newpurchase, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(SelectProducts(int,int)));
    //connect(newpurchase, SIGNAL(itemChanged(QTableWidgetItem*)),this, SLOT(checkProduct(QTableWidgetItem*)));
    connect(newpurchase, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(setTotalValue(QTableWidgetItem*)));
    //connect(newpurchase, SIGNAL(itemChanged(QTableWidgetItem*)), this, SLOT(updatetotalplabel(QTableWidgetItem*)));
    connect(addrow, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(removerow, SIGNAL(clicked()), this, SLOT(removeRow()));
    connect(printbutton,SIGNAL(clicked()), this, SLOT(savePurchase()));
}

//sets up a table for collecting new products.
void MainWindow::on_NewPurchases_pushButton_clicked()
{

    setup_NewPurchaseAndSale("Add purchase", "Cancel purchase");


}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PURCHASE TABLE SLOTS.
///
///


bool MainWindow::checkProduct(QString productname)
{
    int productid = WorkerFunctions::getId(productname.toUpper());

    if(productid == 0)
        return  false;
    else
        return true;
}

//Select products.
void MainWindow::SelectProducts(int x, int y)
{

   if(y == 1 && !newpurchase->item(x, y))
    {
        bool ok;
        double price = QInputDialog::getDouble(this, "Price", "Price", 1,0 , 10000000000000000, 2, &ok );;

        newpurchase->setItem(x,y, new QTableWidgetItem(QString("%1").arg(price)));

    }
}

//set total value
void MainWindow::setTotalValue(QTableWidgetItem* item)
{
    if(item->column() == 0)
    {
        QString productname = item->text();
        bool productexistance = checkProduct(productname);

        if(!productexistance)
        {
            QMessageBox::critical(this, "New transaction", "Product not in system. \n PLEASE FILL IN A PRODUCT THAT IS IN THE SYSTEM.");
            return;
        }
        if(addrow->text() == "Add sale")
        {
            double price = WorkerFunctions::getUnitPrice(productname.toUpper());
            newpurchase->setItem(item->row(),1, new QTableWidgetItem(QString("%1").arg(price)));
        }
    }
    else if(item->column() == 1)
    {
        double price;
        double quantity;
        bool ok;

        price = item->text().toDouble();
        quantity = QInputDialog::getDouble(this, "Quantity", "Quantity", 1,0 , 100000000000000000, 2, &ok );

        newpurchase->setItem(item->row(), 2, new QTableWidgetItem(QString("%1").arg(quantity)));
        double total = price * quantity;
        newpurchase->setItem(item->row(), 3, new QTableWidgetItem(QString("%1").arg(total)));

    }else if(item->column() == 3)
    {
        double price;
        double temprice;
        price = item->text().toDouble();
        temprice = showtotalprice->text().toDouble();
        temprice = temprice + price;
        showtotalprice->setText(QString("%1").arg(temprice));


    }
}

//saves purchases and adds to stock
void MainWindow::savePurchase()
{
    QString saleorpurchase;
    if(addrow->text() == "Add sale")
        saleorpurchase = "Sale";
    else
        saleorpurchase = "Purchase";

    int numberoftransactions = WorkerFunctions::getSavedCount(saleorpurchase+"s");
    qDebug() << numberoftransactions;
    if(!(numberoftransactions <=29) ){
        QMessageBox::information(this, "save ransaction", "This is a trial version. You can not save more than 30 transactions");
        return;
    }else{
    QTableWidgetItem* item;
    bool check = false;
    for(int row=0; row<newpurchase->rowCount(); row++)
    {
        for(int column=0; column<newpurchase->columnCount(); column++)
        {
            item = newpurchase->item(row, column);

            if(!item)
            {
                QMessageBox::critical(this,"Save "+saleorpurchase+" ", "There are blank rows or blank cells. Press cancel "+saleorpurchase+" to delete them or fill them up");
                check = true;
                break;

            }
        }

    }

    if(check)
    {
        return;
    }

    QMessageBox::StandardButton save;

    double totalprice = showtotalprice->text().toDouble();
    QLocale locale;
    QString total = locale.toCurrencyString(totalprice, "Kshs ");
    save = QMessageBox::information(this, "Complete transaction", "Total : " +total+ " Complete " + saleorpurchase + "?", QMessageBox::Yes | QMessageBox::No );

    if(save == QMessageBox::Yes){

        QList<purchasesave> purchasesaves;

        for(int rows=0; rows<newpurchase->rowCount(); ++rows)
        {
            if(!newpurchase->item(rows,0)->text().isEmpty())
            {
                purchasesave temp;

                temp.type = saleorpurchase;

                temp.number = WorkerFunctions::getId(newpurchase->item(rows, 0)->text().toUpper());

                temp.price = newpurchase->item(rows, 1)->text().toInt();


                temp.quantity = newpurchase->item(rows, 2)->text().toDouble();


                temp.totalprice = newpurchase->item(rows, 3)->text().toDouble();

                purchasesaves.append(temp);

                continue;
            }
        }

      WorkerFunctions::savePurchases(purchasesaves);

        bool ok;
        double recievedamount = QInputDialog::getDouble(this, "Complete transaction", "Amount recieved", 1,0 , 100000000000000000, 2, &ok );

        double change = recievedamount - totalprice;
        QString changegiven = locale.toCurrencyString(change, "Kshs ");

        QMessageBox::information(this, "Transaction complete", ""+saleorpurchase+" successful\n Change : " + changegiven);
        showtotalprice->setText("0");

        while(newpurchase->rowCount() > 0)
            newpurchase->removeRow(0);

    }

    }
}

//adds a new purchase row
void MainWindow::addRow()
{
    int rowcount = newpurchase->rowCount();
    newpurchase->insertRow(rowcount);
}

//removes a row from purchase table.
void MainWindow::removeRow()
{
    bool ok;
    int row = QInputDialog::getInt(this, "Remove row", "Row Number", 1, 0, newpurchase->rowCount(), 1, &ok);


    double current_totalprice = showtotalprice->text().toDouble();

    double remove_amount;

    if(newpurchase->item((row-1),3))
    {
        remove_amount = newpurchase->item((row-1),3)->text().toDouble();

        showtotalprice->setText(QString("%1").arg(current_totalprice-remove_amount));
    }

    newpurchase->removeRow((row-1));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////// END OF PURCHASE TABLE SLOTS ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainWindow::setup_FilterControlArea()
{
    name = new QLineEdit;
    namelabel = new QLabel;
    namelabel->setText("Product name");

    fromlabel = new QLabel;
    fromlabel->setText("From");

    tolabel = new QLabel;
    tolabel->setText("To");

    fromdate = new QDateEdit;
    fromdate->setMinimumHeight(45);
    fromdate->setDisplayFormat("dd-MMM-yyyy");
    fromdate->setDate(QDate::currentDate());
    fromdate->setCalendarPopup(true);
    todate = new QDateEdit;
    todate->setDisplayFormat("dd-MMM-yyyy");
    todate->setDate(QDate::currentDate());
    todate->setCalendarPopup(true);
    todate->setMinimumHeight(45);

    filterbutton = new QPushButton;
    filterbutton->setText("Filter");
    filterbutton->setMinimumWidth(60);
    filterbutton->setMaximumHeight(40);
    filterbutton->setMinimumHeight(40);

    filtertext = new QLabel;
    filtertext->setMinimumWidth(300);
    filtertext->setWordWrap(true);
    filtertext->setObjectName("filtertext");
    filtertext->hide();

    purchaseframe = new QFrame;
    smalllayout = new QGridLayout;
    smalllayout->addWidget(namelabel, 0, 0);
    smalllayout->addWidget(name, 0,1);
    smalllayout->addWidget(fromlabel,1,0);
    smalllayout->addWidget(fromdate,1,1);
    smalllayout->addWidget(tolabel, 2, 0);
    smalllayout->addWidget(todate, 2,1);
    smalllayout->addWidget(filterbutton, 3,1);
    smalllayout->addWidget(filtertext,4,0);


    purchaseframe->setLayout(smalllayout);
    ui->controlarea_VL->addWidget(purchaseframe,10,Qt::AlignTop);


}

void MainWindow::viewPurchaseAndSale(QString table)
{
    controlLayouts(ui->workspacearea_VL, ui->controlarea_VL);

    if(table == "Purchases")
        ui->Info_label->setText("Viewing purchases");
    else if(table == "Sales")
        ui->Info_label->setText("Viewing sales");

    WorkerFunctions::openConnection();

    model = new QSqlRelationalTableModel;
    model->setTable(table);
    model->removeColumn(0);
    model->setRelation(0, QSqlRelation("Products", "Id", "Name"));
    model->select();

    model->setHeaderData(0,Qt::Horizontal, "Product");
    model->setHeaderData(1,Qt::Horizontal, "Price per unit");
    model->setHeaderData(2,Qt::Horizontal, "Quantity");
    model->setHeaderData(3,Qt::Horizontal, "Total price");
    model->setHeaderData(4,Qt::Horizontal, "Date");

    purchases = new QTableView;
    purchases->setStyleSheet("background-color:transparent;");
    purchases->setAlternatingRowColors(true);
    purchases->setEditTriggers(QAbstractItemView::NoEditTriggers);
    purchases->setModel(model);
    purchases->setItemDelegateForColumn(1,new TableFormat(purchases));
    purchases->setItemDelegateForColumn(2,new TableFormat(purchases));
    purchases->setItemDelegateForColumn(3,new TableFormat(purchases));
    ui->workspacearea_VL->addWidget(purchases);

    setup_FilterControlArea();
    connect(filterbutton, SIGNAL(clicked()), this, SLOT(purchaseFilter()));

    WorkerFunctions::closeConnection();
}

//displays all content from table purchases.
void MainWindow::on_ViewPurchase_pushButton_clicked()
{

    table = "Purchases";
    viewPurchaseAndSale(table);
}

void MainWindow::purchaseFilter()
{
    QDate fromdte = fromdate->date();
    QDate todte = todate->date();
    QString product = name->text().toUpper();
    int productid = WorkerFunctions::getId(product);

    if(!product.isEmpty() && productid==0)
    {
      QMessageBox::warning(this," ", "Product not in the system");
      return;
    }

    controlLayouts(ui->workspacearea_VL);

    WorkerFunctions::openConnection();
    model = new QSqlRelationalTableModel;
    model->setTable(table);
    model->removeColumn(0);
    model->setRelation(0, QSqlRelation("Products", "Id", "Name"));
    model->setHeaderData(0,Qt::Horizontal, "Product");
    model->setHeaderData(1,Qt::Horizontal, "Price per unit");
    model->setHeaderData(2,Qt::Horizontal, "Quantity");
    model->setHeaderData(3,Qt::Horizontal, "Total price");
    model->setHeaderData(4,Qt::Horizontal, "Date");

    QString filter;
    QString filtermessage;

    if(product.isEmpty()){
        filter = QString("date BETWEEN '%1' AND '%2'").arg(fromdte.toString("yyyy-MM-dd")).arg(todte.toString("yyyy-MM-dd"));

        filtermessage = QString("Showing all "+table+" between %1 and %2").arg(fromdte.toString()).arg(todte.toString());
    }
    else
    {

        filter = QString("productid = %1 AND date BETWEEN '%2' AND '%3'").arg(productid).arg(fromdte.toString("yyyy-MM-dd")).arg(todte.toString("yyyy-MM-dd"));

        filtermessage = QString("Showing all %1 "+table+" between %2 and %3").arg(product).arg(fromdte.toString()).arg(todte.toString());
    }

    model->setFilter(filter);
    model->select();
    qDebug() << model->lastError().text();
    purchases = new QTableView;
    purchases->setAlternatingRowColors(true);
    purchases->setStyleSheet("background-color:transparent;");
    purchases->setEditTriggers(QAbstractItemView::NoEditTriggers);
    purchases->setModel(model);
    purchases->setItemDelegateForColumn(1, new TableFormat(purchases));
    purchases->setItemDelegateForColumn(2, new TableFormat(purchases));
    purchases->setItemDelegateForColumn(3, new TableFormat(purchases));
    ui->workspacearea_VL->addWidget(purchases);

    QString summary = purchaseAndSaleSummary();
    filtertext->setText(filtermessage + summary);
    filtertext->show();


    WorkerFunctions::closeConnection();


}

QString MainWindow::purchaseAndSaleSummary()
{
    double price = 0.00;
    double quantity = 0.00;
    for(int row=0; row<model->rowCount(); row++)
    {
        price = price + model->index(row, 1).data().toDouble();
        quantity = quantity + model->index(row,2).data().toDouble();
    }

    QLocale locale;
    QString totalprice = locale.toCurrencyString(price,"Kshs ");
    QString totalquantity = locale.toCurrencyString(quantity, " ");

    QString summary = QString("\nQuantity: " + totalquantity + "\nTotal price: " + totalprice);

    return summary;


}

void MainWindow::filterProduct(QStringList products)
{
    bool ok;
    QString product = QInputDialog::getItem(this, "Products", "Select product", products, 0, false, &ok);

    name->setText(product);
}

void MainWindow::setup_ViewStockAndProducts()
{
    searchstockbtn = new QPushButton;
    searchstockbtn->setMaximumWidth(200);
    searchstockbtn->setMinimumHeight(40);
    searchstockbtn->setText("Go");

    searchstock = new QLineEdit;
    searchstock->setPlaceholderText("Search...");

    smalllayout = new QGridLayout;
    smalllayout->addWidget(searchstock, 1, 0);
    smalllayout->addWidget(searchstockbtn, 2,0);

    purchaseframe = new QFrame;
    purchaseframe->setLayout(smalllayout);

    ui->controlarea_VL->addWidget(purchaseframe,10, Qt::AlignTop);
}

//view stock.
void MainWindow::on_ViewStock_pushButton_clicked()
{
    ui->Info_label->setText("Viewing stock");
    controlLayouts(ui->controlarea_VL,ui->workspacearea_VL);

    setup_ViewStockAndProducts();

    WorkerFunctions::openConnection();

    model = new QSqlRelationalTableModel;
    model->setTable("Stock");
    model->removeColumn(0);
    model->setRelation(0, QSqlRelation("Products", "id", "Name"));
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Product"));
    model->setHeaderData(1, Qt::Horizontal, tr("Quantity"));

    purchases = new QTableView;
    purchases->setAlternatingRowColors(true);
    purchases->setStyleSheet("background-color:transparent;");
    purchases->setEditTriggers(QAbstractItemView::NoEditTriggers);
    purchases->setModel(model);
    for(int column=0; column<2; column++){
        purchases->setColumnWidth(column, 200);
    }
    ui->workspacearea_VL->addWidget(purchases);

    connect(searchstockbtn, SIGNAL(clicked()), this, SLOT(returnsearchstock()));

    WorkerFunctions::closeConnection();

}

void MainWindow::returnsearchstock()
{
    QString searchProduct = searchstock->text().toUpper();

    if(searchProduct.isEmpty())
        QMessageBox::warning(this, "Stock", "Please enter a product");
    else
    {
        int productid = WorkerFunctions::getId(searchProduct);

        double quantity = WorkerFunctions::getStockQuantity(productid);

        QLocale local;

        if(quantity == 0.00)
            QMessageBox::warning(this, "Stock", "Product not found");
        else
            QMessageBox::information(this, "Stock", QString("PRODUCT : %1 \n QUANTITY : %2").arg(searchProduct).arg(local.toCurrencyString(quantity, " ")));
    }
}

//SALES
void MainWindow::on_NewSale_pushButton_clicked()
{
    setup_NewPurchaseAndSale("Add sale", "Cancel sale");

}


void MainWindow::on_ViewSales_pushButton_clicked()
{
    table = "Sales";
    viewPurchaseAndSale(table);
}

void MainWindow::on_Report_pushButton_clicked()
{
    ui->Info_label->setText("Viewing report");
    controlLayouts(ui->controlarea_VL, ui->workspacearea_VL);

    setup_FilterControlArea();

    connect(filterbutton, SIGNAL(clicked()),this, SLOT(setup_ReportTables()));

}

void MainWindow::setup_ReportTables()
{
    controlLayouts(ui->workspacearea_VL);


    QString productname = name->text().toUpper();
    int productid = WorkerFunctions::getId(productname);


        QDate reportfromdate = fromdate->date();
        QDate reporttodate = todate->date();

        WorkerFunctions::openConnection();

        purchasemodel = new QSqlRelationalTableModel(this);
        purchasemodel->setTable("purchases");
        purchasemodel->removeColumn(0);
        purchasemodel->setRelation(0, QSqlRelation("Products", "Id", "Name"));
        purchasemodel->setHeaderData(0,Qt::Horizontal, "Product");
        purchasemodel->setHeaderData(1,Qt::Horizontal, "Price per unit");
        purchasemodel->setHeaderData(2,Qt::Horizontal, "Quantity");
        purchasemodel->setHeaderData(3,Qt::Horizontal, "Total price");
        purchasemodel->setHeaderData(4,Qt::Horizontal, "Date");

        QString purchasereportfilter;
        QString salereportfilter;
        if(name->text().isEmpty())
        {
           purchasereportfilter = QString("date BETWEEN '%1' AND '%2'").arg(reportfromdate.toString("yyyy-MM-dd")).arg(reporttodate.toString("yyyy-MM-dd"));

           salereportfilter = QString("date BETWEEN '%1' AND '%2'").arg(reportfromdate.toString("yyyy-MM-dd")).arg(reporttodate.toString("yyyy-MM-dd"));

        }else
        {
           purchasereportfilter = QString("productid = %1 AND date BETWEEN '%2' AND '%3'").arg(productid).arg(reportfromdate.toString("yyyy-MM-dd")).arg(reporttodate.toString("yyyy-MM-dd"));

           salereportfilter = QString("productid = %1 AND date BETWEEN '%2' AND '%3'").arg(productid).arg(reportfromdate.toString("yyyy-MM-dd")).arg(reporttodate.toString("yyyy-MM-dd"));
        }


        purchasemodel->setFilter(purchasereportfilter);
        purchasemodel->select();
        qDebug() << purchasemodel->lastError();
        purchasereport = new QTableView(this);
        purchasereport->setStyleSheet("background-color:transparent;");
        purchasereport->setAlternatingRowColors(true);
        purchasereport->setEditTriggers(QAbstractItemView::NoEditTriggers);
        purchasereport->setItemDelegateForColumn(1, new TableFormat(purchasereport));
        purchasereport->setItemDelegateForColumn(2, new TableFormat(purchasereport));
        purchasereport->setItemDelegateForColumn(3, new TableFormat(purchasereport));
        purchasereport->setModel(purchasemodel);

        salemodel = new QSqlRelationalTableModel(this);
        salemodel->setTable("sales");
        salemodel->removeColumn(0);
        salemodel->setRelation(0, QSqlRelation("Products", "Id", "Name"));
        salemodel->setHeaderData(0,Qt::Horizontal, "Product");
        salemodel->setHeaderData(1,Qt::Horizontal, "Price per unit");
        salemodel->setHeaderData(2,Qt::Horizontal, "Quantity");
        salemodel->setHeaderData(3,Qt::Horizontal, "Total price");
        salemodel->setHeaderData(4,Qt::Horizontal, "Date");


        salemodel->setFilter(salereportfilter);
        salemodel->select();
        qDebug() << salemodel->lastError();

        salereport = new QTableView(this);
        salereport->setStyleSheet("background-color:transparent;");
        salereport->setAlternatingRowColors(true);
        salereport->setEditTriggers(QAbstractItemView::NoEditTriggers);
        salereport->setItemDelegateForColumn(1, new TableFormat(salereport));
        salereport->setItemDelegateForColumn(2, new TableFormat(salereport));
        salereport->setItemDelegateForColumn(3, new TableFormat(salereport));
        salereport->setModel(salemodel);

        ui->workspacearea_VL->addWidget(purchasereport);
        ui->workspacearea_VL->addWidget(salereport);


        setup_ReportSummary();

        WorkerFunctions::closeConnection();


}

void MainWindow::setup_ReportSummary()
{

    double purchasestotal = 0.00;
    double purchasequantitytotal = 0.00;

    double saletotal = 0.00;
    double salequantitytotal = 0.00;

    QString productname;
    if(name->text().isEmpty())
        productname = "All";
    else
        productname = name->text();


    for(int i=0; i<purchasemodel->rowCount(); i++)
    {
        QModelIndex quantity = purchasemodel->index(i,2, QModelIndex());
        QModelIndex price = purchasemodel->index(i,3, QModelIndex());

        purchasequantitytotal += purchasereport->model()->data(quantity).toDouble();
        purchasestotal += purchasereport->model()->data(price).toDouble();

    }

    for(int i=0; i<salemodel->rowCount(); i++)
    {
        QModelIndex quantity = salemodel->index(i,2, QModelIndex());
        QModelIndex price = salemodel->index(i,3, QModelIndex());

        salequantitytotal += salereport->model()->data(quantity).toDouble();
        saletotal += salereport->model()->data(price).toDouble();

    }

    double itemsremaining = purchasequantitytotal - salequantitytotal;

    filtertext->show();
    QLocale currency;
    QString totalpurhase = currency.toCurrencyString(purchasestotal, "Kshs ");
    QString totalsale = currency.toCurrencyString(saletotal, "Kshs ");
    QString fullreport = QString("Table 1 PURCHASES Table 2 SALES \n"
                                 "    Report. \n"
                                 "Product : %1 \n"
                                 "Purchases total quantity : %2\n"
                                 "Purchases total price : %3 \n"
                                 "Sales total quantity : %4 \n"
                                 "Sales total price : %5 \n"
                                 "Items remaining in stock from this period's transactions: %6").arg(productname).arg(purchasequantitytotal).arg(totalpurhase).arg(salequantitytotal).arg(totalsale).arg(itemsremaining);
    filtertext->setText(fullreport);
}

void MainWindow::on_action_NewSale_triggered()
{
    on_NewSale_pushButton_clicked();
}

void MainWindow::on_action_NewPurchase_triggered()
{
    on_NewPurchases_pushButton_clicked();
}

void MainWindow::on_action_NewProduct_triggered()
{
    on_NewProduct_pushButton_clicked();
}

void MainWindow::on_action_ViewSales_triggered()
{
    on_ViewSales_pushButton_clicked();
}

void MainWindow::on_action_ViewPurchases_triggered()
{
    on_ViewPurchase_pushButton_clicked();
}


void MainWindow::on_action_ViewProducts_triggered()
{
    on_ViewProduct_pushButton_clicked();
}

void MainWindow::on_action_ViewStock_triggered()
{
    on_ViewStock_pushButton_clicked();
}

void MainWindow::on_action_ViewReport_triggered()
{
    on_Report_pushButton_clicked();
}

void MainWindow::setup_UserControlArea()
{
    username_label = new QLabel;
    password_label = new QLabel;

    username_label->setText("Username");
    password_label->setText("Password");

    username_edit = new QLineEdit;
    username_edit->setToolTip("Not more than 20 characters");
    username_edit->setToolTipDuration(5000);
    password_edit = new QLineEdit;
    password_edit->setToolTip("Not more than 8 characters, not less than 6 characters");
    password_edit->setToolTipDuration(5000);
    confirmpassword_edit = new QLineEdit;
    confirmpassword_edit->setPlaceholderText("Confirm password");

    password_edit->setEchoMode(QLineEdit::Password);
    confirmpassword_edit->setEchoMode(QLineEdit::Password);

    saveuser_btn = new QPushButton;
    saveuser_btn->setMinimumHeight(40);

    is_superuser = new QCheckBox("Super user");
    is_superuser->setMinimumHeight(25);

    QRegExpValidator* username_validator = new QRegExpValidator(QRegExp("[0-9A-Za-z' ']{1,20}"),username_edit);
    QRegExpValidator* password_validator = new QRegExpValidator(QRegExp("[0-9A-Za-z~!@#$%^&*_+]{6,8}"),password_edit);

    username_edit->setValidator(username_validator);
    password_edit->setValidator(password_validator);
    confirmpassword_edit->setValidator(password_validator);


    smalllayout = new QGridLayout;
    smalllayout->addWidget(username_label,0,0);
    smalllayout->addWidget(username_edit,0,1);
    smalllayout->addWidget(password_label,1,0);
    smalllayout->addWidget(password_edit,1,1);
    smalllayout->addWidget(confirmpassword_edit,2,1);
    smalllayout->addWidget(is_superuser,3,1);
    smalllayout->addWidget(saveuser_btn,4,1);

    purchaseframe = new QFrame;
    purchaseframe->setLayout(smalllayout);

    ui->controlarea_VL->addWidget(purchaseframe,10,Qt::AlignTop);

}

void MainWindow::on_action_NewUser_triggered()
{
    extern int user_id;
    bool superuser = WorkerFunctions::is_superuser(user_id);

    if(superuser){
        controlLayouts(ui->controlarea_VL, ui->workspacearea_VL);
        setup_UserControlArea();
        ui->Info_label->setText("New user");
        saveuser_btn->setText("Save user");

        connect(saveuser_btn,SIGNAL(clicked()),this, SLOT(saveNewUser()));

    }else
    {
        QMessageBox::warning(this, "Create user", "Your are not logged in as super user. You can not add users");
    }


}

void MainWindow::saveNewUser()
{
    QString username = username_edit->text();
    QString password = password_edit->text();
    QString password_confirmed = confirmpassword_edit->text();

    if(username.isEmpty() || password.isEmpty() || password_confirmed.isEmpty())
    {
        QMessageBox::critical(this, "Add user", "Username, password and confirm password must be filled");
        return;
    }

    if(password != password_confirmed)
        QMessageBox::warning(this, "Save user", "Passwords do not match.");
    else
    {
        QMessageBox::StandardButton save;

        save = QMessageBox::question(this,"Save user", "Save user "+username+"? ", QMessageBox::Yes | QMessageBox::No);

        if(save == QMessageBox::Yes)
        {
            bool saved = WorkerFunctions::saveUser(username.toUpper(), password, is_superuser->isChecked());

            if(saved)
            {
                QMessageBox::information(this, "Save user", "user Saved");
                username_edit->clear();
                password_edit->clear();
                confirmpassword_edit->clear();
                is_superuser->setChecked(false);
            }else
            {
                QMessageBox::warning(this, "Save user", "Username taken. Please pick another username");
                return;
            }

        }


    }
}

void MainWindow::on_actionMODIFY_triggered()
{
    controlLayouts(ui->controlarea_VL, ui->workspacearea_VL);


    setup_UserControlArea();
    username_label->setText("Previous password");
    username_edit->setEchoMode(QLineEdit::Password);
    password_label->setText("New password");
    saveuser_btn->setText("Save changes");

    extern int user_id;
    bool user_status = WorkerFunctions::is_superuser(user_id);

    if(!user_status)
        is_superuser->hide();


    QString session_user = ui->attendant_label->text().remove(0,12);
    ui->Info_label->setText("Logged in as " + session_user);

    connect(saveuser_btn, SIGNAL(clicked()), this, SLOT(modifyAccount()));

}

void MainWindow::modifyAccount()
{
    QString previous_pass = username_edit->text();
    QString newpass = password_edit->text();
    QString newpass_confirm = confirmpassword_edit->text();
    bool user_status = false;

    if(is_superuser)
        user_status = is_superuser->isChecked();

    if(newpass != newpass_confirm)
    {
      QMessageBox::warning(this, "Modify account", "New passwords do not match");
      return;
    }

    bool is_modified = WorkerFunctions::modifyAccount(previous_pass, newpass, user_status);

    if(is_modified)
    {
        QMessageBox::information(this, "Modify account", "Account modified");
        username_edit->clear();
        password_edit->clear();
        confirmpassword_edit->clear();
    }
    else
    {
        QMessageBox::warning(this, "Modify account", "wrong previous password");
    }


}


void MainWindow::on_actionDELETE_triggered()
{
    ui->Info_label->setText("Delete user");
    controlLayouts(ui->controlarea_VL, ui->workspacearea_VL);

    salemodel = new QSqlRelationalTableModel(this);
    salemodel->setTable("users");
    salemodel->removeColumn(0);
    salemodel->removeColumn(1);
    salemodel->select();

    userlist = new QListView;
    userlist->setModel(salemodel);

    ui->workspacearea_VL->addWidget(userlist);

   // connect(userlist, SIGNAL(Clicked(QModelIndex)), this, SLOT(deleteUser(QModelIndex)));

    connect(userlist,SIGNAL(clicked(QModelIndex)),this, SLOT(deleteUser(QModelIndex)));

}

void MainWindow::deleteUser(QModelIndex index)
{
    extern int user_id;
    bool superuser = WorkerFunctions::is_superuser(user_id);


    QString delete_user = index.data().toString();
    QString session_user = ui->attendant_label->text().remove(0,12);
    if(superuser)
    {
      QMessageBox::StandardButton deleteuser;
      deleteuser = QMessageBox::question(this, "Delete user", "Are you sure you want to delete user " + delete_user + " ?", QMessageBox::Yes | QMessageBox::No);

      if(deleteuser == QMessageBox::Yes)
      {
          if(session_user == delete_user)
          {
              QMessageBox::warning(this, "Delete user", "You cannot delete the account you are currently logged in using");
          }else
          {
              WorkerFunctions::deleteUser(delete_user.toUpper());
              QMessageBox::information(this, "Delete user", "Deleted");
              on_actionDELETE_triggered();
          }
      }
    }else
    {
        QMessageBox::warning(this, "Delete user", "Your are not logged in as super user. You can not delete users");
    }

}

void MainWindow::on_action_About_triggered()
{
    About* about_dialog = new About(this);
    about_dialog->setModal(true);
    about_dialog->exec();

}

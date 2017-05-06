#include "workerfunctions.h"
#include <QtCore>
#include <QVBoxLayout>
#include <QSqlQuery>
#include "mainwindow.h"
#include "global_var.h"


//constructor
WorkerFunctions::WorkerFunctions()
{
}

//destructor
WorkerFunctions::~WorkerFunctions()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// \ DATABASE CONNECTIVITY FUNCTIONS
///
///
//returns database driver
QSqlDatabase WorkerFunctions::driver()
{
    QSqlDatabase sqlitedb;
    return sqlitedb;
}

//opens database connection
void WorkerFunctions::openConnection()
{
    QSqlDatabase sqlitedb;
    QString dbpath = QDir::currentPath() + "/bma.db";
    sqlitedb = QSqlDatabase::addDatabase("QSQLITE");
    sqlitedb.setDatabaseName(dbpath);

    sqlitedb.open();
}

//closes database connection.
void WorkerFunctions::closeConnection()
{
    QSqlDatabase sqlitedb;
    sqlitedb.close();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////// LOGIN SQL IMPLEMENTATION
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//authenticate
bool WorkerFunctions::loginAuthenticate(QString username, QString password)
{
    int count = 0;

    extern int user_id;

    QSqlDatabase sqlitedb;

    openConnection();
    QSqlQuery qry(sqlitedb);

    qry.prepare("SELECT * FROM users WHERE Username = '"+username.toUpper()+"' AND Password = '"+passwordHash(password)+"' LIMIT 1 ");

    if(qry.exec())
    {
        while(qry.next())
        {
            user_id = qry.value(0).toInt();
            count++;
        }
    }



    closeConnection();
    if(count >=1)
        return true;
    else
        return false;

}

QString WorkerFunctions::returnUsername()
{
    QString user_name;

    return user_name;
}

QString WorkerFunctions::getUser(int userid)
{
    QSqlDatabase sqlitedb;
    QString username;

    openConnection();
    QSqlQuery qry(sqlitedb);

    qry.prepare("SELECT * FROM users WHERE id = :id");
    qry.bindValue(":id", userid);

    qry.exec();
    while (qry.next()) {

        username = qry.value(1).toString();
    }

    closeConnection();

    return username;
}

bool WorkerFunctions::is_superuser(int userid)
{
    bool superuser = false;
    QSqlDatabase sqlitedb;
    openConnection();
    QSqlQuery qry(sqlitedb);

    qry.prepare("SELECT * FROM users WHERE id = :id");
    qry.bindValue(":id", userid);

    qry.exec();

    while(qry.next())
    {
        superuser = qry.value(3).toBool();
    }

    return superuser;
}

void WorkerFunctions::deleteUser(QString username)
{
    QSqlDatabase sqlitedb;
    openConnection();
    QSqlQuery qry(sqlitedb);

    qry.prepare("DELETE FROM users WHERE username = :name");
    qry.bindValue(":name", username);
    qry.exec();
    closeConnection();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////// PRODUCTS SQL IMPLMENTATIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//saves products
bool WorkerFunctions::saveProduct(QString name, double price)
{
    QStringList products = pullProducts();

    if(products.contains(name))
    {
        return false;

    }

    QSqlDatabase sqlitedb;
    openConnection();
    QSqlQuery qry(sqlitedb);

    qry.prepare("INSERT INTO Products(Name, Price) VALUES(:name, :price)");
    qry.bindValue(":name", name);
    qry.bindValue(":price", price);
    qry.exec();
    closeConnection();
    addToStock(name);
    return true;
}

//adds the new product to table stock
void WorkerFunctions::addToStock(QString name)
{
    int id = getId(name);

    QSqlDatabase sqlitedb;
    openConnection();
    QSqlQuery qry(sqlitedb);

    qry.prepare("INSERT INTO stock (productid, quantity) VALUES(:productid, :quantity)");
    qry.bindValue(":productid", id);
    qry.bindValue(":quantity", 0.0);

    if(qry.exec())
        qDebug() << "Boom";
    else
        qDebug() << "Feck";

   closeConnection();
}

//updates an already existing product
void WorkerFunctions::updateProduct(int id, int price)
{
    QSqlDatabase sqlitedb;
    openConnection();
    QSqlQuery qry(sqlitedb);

    qry.prepare("UPDATE products SET price = :price WHERE id = :id");
    qry.bindValue(":price", price);
    qry.bindValue(":id", id);


    closeConnection();
}

//deletes an existing product
void WorkerFunctions::deleteProduct(int id)
{
    QSqlDatabase sqlitedb;
    openConnection();
    QSqlQuery qry(sqlitedb);

    qry.prepare("DELETE FROM products WHERE ID = :id");
    qry.bindValue(":id", id);

    qry.exec();

    closeConnection();
}
// returns list of product names.
QSqlQueryModel* WorkerFunctions::returnModel(QSqlQueryModel *model)
{
    QSqlDatabase sqlitedb;
    openConnection();

    QSqlQuery qry(sqlitedb);
    qry.exec("SELECT name FROM products");
    model->setQuery(qry);

    closeConnection();
    return model;
}

QStringList WorkerFunctions::returnValues(QString name)
{
    QStringList productdetails;
    QSqlDatabase sqlitedb;
    openConnection();
    QSqlQuery qry(sqlitedb);

    qry.prepare("SELECT * FROM products WHERE name = :name LIMIT 1");
    qry.bindValue(":name", name);
    qry.exec();

    while(qry.next())
    {
        productdetails.append(qry.value(0).toString());
        productdetails.append(qry.value(1).toString());
        productdetails.append(qry.value(2).toString());
    }

    closeConnection();
    return productdetails;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// MAINWINDOW SQL IMPLEMENTATIONS
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//picks product names for the purchases table view.
QStringList WorkerFunctions::pullProducts()
{
    QStringList products;
    QSqlDatabase sqlitedb;
    openConnection();

    QSqlQuery qry(sqlitedb);

    qry.prepare("SELECT Name FROM Products");

    if(qry.exec())
    {
        while(qry.next())
        {
            products.append(qry.value(0).toString());
        }
    }

    closeConnection();
    return products;
}

//returns product id for saving to stock
int WorkerFunctions::getId(QString product_name)
{
    int id;
    QSqlDatabase sqlitedb;
    openConnection();

    QSqlQuery qry(sqlitedb);
    qry.prepare("SELECT id FROM products WHERE name = :name");
    qry.bindValue(":name",product_name);
    qry.exec();

        if(qry.next())
        {
            id = qry.value(0).toInt();
        }else
        {
            id = 0;
        }


    closeConnection();

    return id;
}
//saves purchases to purchase table
void WorkerFunctions::savePurchases(QList<purchasesave> purchasesaves)
{
    QSqlDatabase sqlitedb;
    openConnection();
    QSqlQuery qry(sqlitedb);

    foreach (purchasesave purchase, purchasesaves) {

        qry.prepare("INSERT INTO "+purchase.type+"s (productid, PricePerUnit, Quantity, TotalPrice, Date) VALUES(:first, :second, :third, :forth, :fifth)");

        qry.bindValue(":first",purchase.number);
        qry.bindValue(":second", purchase.price);
        qry.bindValue(":third",purchase.quantity);
        qry.bindValue(":forth", purchase.totalprice);
        qry.bindValue(":fifth",QDate::currentDate());

        if(qry.exec())
            continue;
        else
            qDebug() << sqlitedb.lastError();

    }

    closeConnection();

    updateStock(purchasesaves);
}

//adds purchases to stock table.
void WorkerFunctions::updateStock(QList<purchasesave> products)
{

    QHash<int,double> insertstock;
    QSqlDatabase sqlitedb;
    openConnection();

    QSqlQuery qry(sqlitedb);

    foreach(purchasesave product, products)
    {
        qry.prepare("SELECT * FROM stock WHERE productid = :productid");
        qry.bindValue(":productid", product.number);

        if(qry.exec())
        {
            while(qry.next())
            {
                if(product.type == "Purchase")
                   insertstock.insert(qry.value(0).toInt(), qry.value(2).toDouble()+product.quantity);
                else
                   insertstock.insert(qry.value(0).toInt(), qry.value(2).toDouble()-product.quantity);
            }
        }else
        {
            qDebug() << sqlitedb.lastError().text();
        }
    }

    QHashIterator<int, double>iterator(insertstock);
    while(iterator.hasNext())
    {
        iterator.next();
        qry.prepare("UPDATE stock SET quantity = :quantity WHERE id = :id");
        qry.bindValue(":quantity", iterator.value());
        qry.bindValue(":id", iterator.key());

        if(qry.exec())
            qDebug() << "Boom";
        else
            qDebug() << qry.lastError().text();
    }


    closeConnection();
}

double WorkerFunctions::getStockQuantity(int id)
{
    double quantity;
    QSqlDatabase sqlitedb;
    openConnection();

    QSqlQuery qry(sqlitedb);

    qry.prepare("SELECT quantity FROM stock WHERE productid =  :id");
    qry.bindValue(":id", id);

    qry.exec();
    if(qry.next())
    {
       quantity = qry.value(0).toDouble();
    }else
        quantity = 0.00;


    closeConnection();
    return quantity;
}

int WorkerFunctions::getUnitPrice(QString product)
{
    int priceperunit;

    QSqlDatabase sqlitedb;
    openConnection();

    QSqlQuery qry(sqlitedb);

    qry.prepare("SELECT price FROM products WHERE name = :name LIMIT 1");
    qry.bindValue(":name", product);

    if(qry.exec())
    {
        while(qry.next())
        {
            priceperunit = qry.value(0).toInt();
        }
    }

    closeConnection();
    return priceperunit;
}

bool WorkerFunctions::saveUser(QString username, QString password, bool is_superuser)
{
    QSqlDatabase sqlitedb;
    openConnection();

    QSqlQuery qry(sqlitedb);

   qry.prepare("INSERT INTO users (username, password, is_superuser) VALUES (:username, :password, :is_superuser)");
    qry.bindValue(":username", username);
    qry.bindValue(":password", passwordHash(password));
    qry.bindValue(":is_superuser", is_superuser);



    if(qry.exec()){
        closeConnection();
        return true;
    }
    else{
        closeConnection();
        return false;
    }


}

QString WorkerFunctions::passwordHash(QString password)
{
    QString passwordhashed;
    for(int character=0; character<password.length(); ++character)
    {
        int unicde = password.at(character).unicode();
        QString part_password;

        if(character +1  == password.length())
            part_password = QString::number(unicde, 16);
        else
            part_password = QString::number(unicde,16) + "&";

        passwordhashed.append(part_password);
    }

    return passwordhashed;
}

QString WorkerFunctions::passwordUnhash(QString passwordhash)
{
    QStringList passwordtemp = passwordhash.split("&");
    QString password;

    foreach (QString chars, passwordtemp) {

        bool converted;

        int unicde = chars.toInt(&converted, 16);

        password.append(QChar(unicde));
    }

    return password;
}


float WorkerFunctions::getProduct(QString productname)
{
    float price;

    QSqlDatabase sqlitedb;
    openConnection();

    QSqlQuery qry(sqlitedb);

    qry.prepare("SELECT * FROM products WHERE name = :name LIMIT 1");
    qry.bindValue(":name", productname);

    qry.exec();

        if(qry.next())
        {
            price = qry.value(2).toFloat();
        }else
        {
            price = 0.00;
        }


    closeConnection();

    return price;
}

bool WorkerFunctions::modifyAccount(QString previouspass, QString newpass, bool user_status)
{
    extern int user_id;
    QSqlDatabase sqlitedb;
    openConnection();

    QSqlQuery qry(sqlitedb);

    qry.prepare("SELECT password, is_superuser FROM users WHERE id = :id");
    qry.bindValue(":id", user_id);

    qry.exec();
    QString passtemp;
    bool is_superuser;
    while(qry.next())
    {
         passtemp = passwordUnhash(qry.value(0).toString());
         is_superuser = qry.value(1).toBool();
    }

    qDebug() << is_superuser;
    if(passtemp != previouspass)
    {
        closeConnection();
        return false;
    }else
    {
        if(is_superuser == user_status)
           qry.prepare("UPDATE users SET password = :password WHERE id = :id");
        else{
            qry.prepare("UPDATE users SET password = :password, is_superuser = :is_superuser WHERE id = :id");
            qry.bindValue(":is_superuser", user_status);
        }

        qry.bindValue(":password", passwordHash(newpass));
        qry.bindValue(":id", user_id);
        if(!qry.exec())
        {
            qDebug() << qry.lastQuery();
            qDebug() << qry.lastError();
        }


        return true;
    }

}

int WorkerFunctions::getSavedCount(QString table){
    QSqlDatabase sqlitedb;
    openConnection();

    QSqlQuery qry(sqlitedb);

    qry.prepare("SELECT COUNT(id) FROM (SELECT id FROM '"+table+"')");
    if(!qry.exec()){
        qDebug() << qry.lastError();
        qDebug() << qry.lastQuery();
    }


    int numberofproducts;
    while(qry.next()){
       numberofproducts = qry.value(0).toInt();
    }
    return numberofproducts;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// END OF MAINWINDOW SQL IMPLEMENTATIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef WORKERFUNCTIONS_H
#define WORKERFUNCTIONS_H
#include <QSqlDatabase>

#include <QObject>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "global_var.h"

class WorkerFunctions
{

public:
    WorkerFunctions();
    ~WorkerFunctions();

    static void openConnection();

    static void closeConnection();

    static bool loginAuthenticate(QString, QString);

    static QStringList pullProducts();

    static bool saveProduct(QString, double);

    static void addToStock(QString);

    static void updateProduct(int, int);

    static void deleteProduct(int);

    static QSqlQueryModel* returnModel(QSqlQueryModel*);

    static QStringList returnValues(QString);

    static QSqlDatabase driver();

    static int getId(QString);

    static void savePurchases(QList<purchasesave>);

    static void updateStock(QList<purchasesave>);

    static double getStockQuantity(int);

    static int getUnitPrice(QString);

    static float getProduct(QString);

    static QString returnUsername();

    static bool saveUser(QString, QString, bool);

    static QString passwordHash(QString);

    static QString passwordUnhash(QString);

    static QString getUser(int);

    static bool is_superuser(int);

    static bool modifyAccount(QString, QString, bool);

    static void deleteUser(QString);

    //trial version limits
    static int getSavedCount(QString table);

public:
    static QSqlDatabase sqlitedb;

};

#endif // WORKERFUNCTIONS_H

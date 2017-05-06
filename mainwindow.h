#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QDate>
#include <QGroupBox>
#include <QDateEdit>
#include <QCheckBox>
#include "addproductsframe.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT



public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    void controlGroupboxes(QGroupBox*);

    void controlLayouts(QVBoxLayout*, QVBoxLayout* layout_two = NULL);

    void purchaseFilters();

    void setup_NewPurchaseAndSale(QString, QString);

    void viewPurchaseAndSale(QString);

    void setup_ReportSummary();

    void setup_FilterControlArea();

    void setup_UserControlArea();

private slots:
    void authenticateUser();

    void on_Sales_commandLinkButton_clicked();

    void on_Purchases_commandLinkButton_clicked();

    void on_Products_commandLinkButton_clicked();

    void on_Stock_commandLinkButton_clicked();

    void on_NewProduct_pushButton_clicked();

    void on_ViewProduct_pushButton_clicked();

    void on_NewPurchases_pushButton_clicked();

    void on_ViewPurchase_pushButton_clicked();

    void SelectProducts(int, int);

    void showTime();

    void setTotalValue(QTableWidgetItem*);

    void savePurchase();

    void addRow();

    void removeRow();

    void purchaseFilter();

    QString purchaseAndSaleSummary();

    void filterProduct(QStringList);

    void setup_ReportTables();

    void on_ViewStock_pushButton_clicked();

    void setup_ViewStockAndProducts();

    void returnsearchstock();

    void returnsearchproduct();

    void saveNewUser();

    void modifyAccount();

    void deleteUser(QModelIndex);

    void on_NewSale_pushButton_clicked();

    void on_ViewSales_pushButton_clicked();

    void on_Report_pushButton_clicked();

    void on_action_NewSale_triggered();

    void on_action_NewPurchase_triggered();

    void on_action_NewProduct_triggered();

    void on_action_ViewSales_triggered();

    void on_action_ViewPurchases_triggered();

    void on_action_ViewProducts_triggered();

    void on_action_ViewStock_triggered();

    void on_action_ViewReport_triggered();

    void on_action_NewUser_triggered();

    void on_actionMODIFY_triggered();

    void on_actionDELETE_triggered();


    void on_action_About_triggered();

signals:
    //void showProducts();

public:
    //functions
    void pullProducts(QComboBox*);
    QString itemContent(int, int );
    bool checkProduct(QString);
    void resizeEvent(QResizeEvent *);

    //variables
public:

    QTableWidget* newpurchase;
    QPushButton* printbutton;
    QPushButton* addrow;
    QPushButton* removerow;
    QPushButton* filter;
    QDateEdit* fromdate;
    QDateEdit* todate;
    QLineEdit* name;
    QLabel* fromlabel;
    QLabel* tolabel;
    QLabel* namelabel;
    QLabel* filtertext;
    QPushButton* filterbutton;
    QLineEdit* searchstock;
    QPushButton* searchstockbtn;
    QLineEdit* username_edit;
    QLineEdit* password_edit;
    QLineEdit* confirmpassword_edit;
    QCheckBox* is_superuser;
    Ui::MainWindow *ui;
private:
    QListView* userlist;
    QLabel* password_label;
    QLabel* username_label;
    QPushButton* saveuser_btn;
    QSqlTableModel* tablemodel;
    QTableView* purchases;
    QLabel* showtotalprice;
    QSqlRelationalTableModel* model;
    AddProductsFrame* addproducts;
    QFrame* purchaseframe;
    QGridLayout* smalllayout;
    QLabel* totalprice;
    QSqlQueryModel *querymodel;
    QString table;
    QTableView* salereport;
    QTableView* purchasereport;
    QSqlRelationalTableModel* purchasemodel;
    QSqlRelationalTableModel* salemodel;

};

#endif // MAINWINDOW_H

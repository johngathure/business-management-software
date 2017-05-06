#ifndef ADDPRODUCTSFRAME_H
#define ADDPRODUCTSFRAME_H

#include <QFrame>
#include <QSqlQueryModel>

namespace Ui {
class AddProductsFrame;
}

class AddProductsFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AddProductsFrame(QWidget *parent = 0);
    ~AddProductsFrame();

signals:
    void showProducts();
private slots:
    void on_Cancel_pushButton_clicked();

    void on_Save_pushButton_clicked();

    void on_Update_pushButton_clicked();

    void on_Loadlist_pushButton_clicked();

    void on_Products_comboBox_currentIndexChanged(const QString &arg1);

    void on_Delete_pushButton_clicked();

private:
    Ui::AddProductsFrame *ui;
    QSqlQueryModel model;
};

#endif // ADDPRODUCTSFRAME_H

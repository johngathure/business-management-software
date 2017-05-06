#ifndef ABOUT_H
#define ABOUT_H

#include <QDialog>

namespace Ui {
class About;
}

class About : public QDialog
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

    void setup_about();
    QString get_abouttext();
private slots:
    void on_Close_pushButton_clicked();

private:
    Ui::About *ui;
};

#endif // ABOUT_H

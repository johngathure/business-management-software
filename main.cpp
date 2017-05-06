#include "mainwindow.h"
#include "login.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QFile>
#include <QTextStream>
#include <QSplashScreen>
#include <QTimer>


//function prototypes
QString readFile(QString);
int user_id;

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    MainWindow mainWin;
    QSplashScreen *splash = new QSplashScreen(&mainWin);
    splash->setPixmap(QPixmap(QDir::currentPath()+ "/splash_screeen"));

    splash->show();
    Qt::Alignment bottomRight = Qt::AlignRight | Qt::AlignBottom;
    splash->showMessage(QObject::tr("Setting up the application..."),
    bottomRight, Qt::white);


    QString stylesheet = readFile(":/Appfiles/App.css");
    app.setStyleSheet(stylesheet);

    QTimer::singleShot(4000, splash, SLOT(close()));
    QTimer::singleShot(4010, &mainWin, SLOT(show()));


    return app.exec();
}

QString readFile(QString filename)
{
    QFile cssfile(filename);

    if(!cssfile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Fuck";
    }

    QTextStream in(&cssfile);
    QString css = in.readAll();
    cssfile.close();

    return css;
}


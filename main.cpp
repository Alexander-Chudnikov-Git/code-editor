#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);
    MainWindow window;

    application.setWindowIcon(QIcon(":/ico/image/main.ico"));
    window.show();

    return application.exec();
}

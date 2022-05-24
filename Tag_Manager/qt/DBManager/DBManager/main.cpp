#include "mainwindow.h"

#include <QApplication>
#include "test.h"

#define QT_NO_DEBUG_OUTPUT
int main(int argc, char *argv[])
{
    // test::runPlantTest();
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();
}

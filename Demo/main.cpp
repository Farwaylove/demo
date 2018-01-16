#include <QApplication>
#include "cxmainwindow.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication a(argc, argv);

    CxMainWindow mainWin(0);
    mainWin.showNormal();


    return a.exec();
}


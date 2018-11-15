#include <QtGui>

#include "main.h"
#include "main_window.h"

const char* OrganizationName    = "MELZ-INVEST";
const char* OrganizationDomain  = "modbus4qt.ru";
const char* ApplicationName     = "modbus4qt TCP Master Demo";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

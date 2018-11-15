#include <QApplication>
#include "main_window.h"

const char* OrganizationName    = "MELZ-INVEST";
const char* OrganizationDomain  = "m-i.ru";
const char* ApplicationName     = "modbus4qt RTU Master Demo";

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationName(OrganizationName);
    QApplication::setOrganizationDomain(OrganizationDomain);
    QApplication::setApplicationName(ApplicationName);

    MainWindow w;
    w.show();

    return a.exec();
}

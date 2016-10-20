#include <QCoreApplication>
#include "system.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    System *sys = new System();
    sys->init();

    return a.exec();
}

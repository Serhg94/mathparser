#include <QCoreApplication>
#include "mathparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    MathParser pm;
    //QString f("-(-104-82)^421+24+(-6+sdfsd(33,24))/2");
    //QString f("-(-104+82)^2");
    //QString f("-20+2*(1.4*(200-50)/3)+3^3");
    QString f("1800-1.1");
    pm.Parse(f);
    bool err = false;
    qDebug() << pm.Calculate(err);
    return a.exec();
}

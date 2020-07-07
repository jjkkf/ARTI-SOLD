#include "singlegame.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SingleGame w;
    w.show();

    return a.exec();
}

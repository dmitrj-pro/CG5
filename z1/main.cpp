#include "draw.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Draw w;
    w.show();

    return a.exec();
}

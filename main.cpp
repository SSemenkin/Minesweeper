#include "field.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Field w;
    w.show();
    return a.exec();
}

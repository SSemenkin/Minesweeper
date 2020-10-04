#include "field.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont (QFont(":/fonts/DroidSansFallback.ttf"));
    Field w;
    w.show();
    return a.exec();
}

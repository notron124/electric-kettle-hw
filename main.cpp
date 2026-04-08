#include "elketwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ElKetWindow w;
    w.show();
    return a.exec();
}

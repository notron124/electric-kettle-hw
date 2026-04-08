#include "elketwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");

    QApplication a(argc, argv);
    ElKetWindow w;
    w.show();
    return a.exec();
}

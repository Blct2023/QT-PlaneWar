#include "mainscene.h"

#include <QApplication>

int Window_Closed = 0;

int main(int argc, char *argv[])
{
    ProgHead:
    Window_Closed = 0;
    QApplication a(argc, argv);
    MainScene w;
    w.show();
    a.exec();
    return 0;
}

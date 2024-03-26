#include "watchers.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WatchersController w;
    w.startWatchers();

    return a.exec();
}

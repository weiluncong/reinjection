#include "workmanager.h"
#include "replay_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ReplayWindow *wind = new ReplayWindow(nullptr);
    wind->show();
    // WorkManager work_;

    return a.exec();
}

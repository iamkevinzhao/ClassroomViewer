#include "main_window.h"
#include <QApplication>
#include "server.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  MainWindow win;
  MapServer server;
  server.map = win.map;
  QObject::connect(
      &win, SIGNAL(OnCalibrated(Pose)), &server, SLOT(OnCalibrated(Pose)));
  win.show();
  return app.exec();
}

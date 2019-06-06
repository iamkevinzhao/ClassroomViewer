#include "main_window.h"
#include <QApplication>
#include "server.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  MainWindow win;
  Server server;
  server.map = win.map;
  win.show();
  return app.exec();
}

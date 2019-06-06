#include "main_window.h"
#include <QApplication>
#include "receiver.h"

int main(int argc, char* argv[])
{
  QApplication app(argc, argv);
  MainWindow win;
  Receiver receiver;
  win.show();
  return app.exec();
}

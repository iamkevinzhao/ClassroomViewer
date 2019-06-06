#include "main_window.h"
#include "ui_main_window.h"
#include "map_widget.h"
#include <QLayout>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  map = new MapWidget(parent);
  setCentralWidget(map);
}

MainWindow::~MainWindow()
{
  delete ui;
}

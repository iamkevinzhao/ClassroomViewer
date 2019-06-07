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
  map->calib_pose.ori.setX(0.0);
  map->calib_pose.ori.setY(1.0);
  map->calib_pose.ori.normalize();
  ui->MapLayout->addWidget(map);
  ui->PositionXEdit->setText(QString::number(map->room.size.x() / 2.0f));
  ui->PositionYEdit->setText(QString::number(map->room.size.y() / 2.0f));
  on_PositionXEdit_editingFinished();
  on_PositionYEdit_editingFinished();
  setWindowTitle("ClassroomViewer");
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::on_CalibrationButton_clicked()
{
  if (!map) {
    return;
  }
  Pose pose;
  bool ok;
  pose.pos.setX(ui->PositionXEdit->text().toFloat(&ok));
  if (!ok) {
    return;
  }
  pose.pos.setY(ui->PositionYEdit->text().toFloat(&ok));
  if (!ok) {
    return;
  }
  pose.ori.setX(0.0f);
  pose.ori.setY(1.0f);
  emit OnCalibrated(pose);
}

void MainWindow::on_PositionXEdit_editingFinished()
{
  if (!map) {
    return;
  }
  bool ok;
  float x = ui->PositionXEdit->text().toFloat(&ok);
  if (!ok) {
    return;
  }
  map->calib_pose.pos.setX(x);
  map->update();
}

void MainWindow::on_PositionYEdit_editingFinished()
{
  if (!map) {
    return;
  }
  bool ok;
  float y = ui->PositionYEdit->text().toFloat(&ok);
  if (!ok) {
    return;
  }
  map->calib_pose.pos.setY(y);
  map->update();
}

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include "map_widget.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  MapWidget* map = nullptr;
signals:
  void OnCalibrated(Pose pose);

private slots:
  void on_CalibrationButton_clicked();

  void on_PositionXEdit_editingFinished();

  void on_PositionYEdit_editingFinished();

private:
  Ui::MainWindow *ui;
};

#endif // MAIN_WINDOW_H

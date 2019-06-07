#ifndef MAP_WIDGET_H
#define MAP_WIDGET_H

#include <QWidget>
#include "models.h"

namespace Ui {
  class MapWidget;
}

class MapWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MapWidget(QWidget *parent = 0);
  ~MapWidget();
  std::vector<Table> tables;
  Pose robot;
  Room room;
  Pose calib_pose;
protected:
  void paintEvent(QPaintEvent *event);
private:
  Ui::MapWidget *ui;
};

#endif // MAP_WIDGET_H

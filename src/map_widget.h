#ifndef MAP_WIDGET_H
#define MAP_WIDGET_H

#include <QWidget>
#include "room_models.h"

namespace Ui {
  class MapWidget;
}

class MapWidget : public QWidget
{
  Q_OBJECT

public:
  explicit MapWidget(QWidget *parent = 0);
  ~MapWidget();

protected:
  void paintEvent(QPaintEvent *event);
private:
  Room room_;
  std::vector<Table> tables_;
  Ui::MapWidget *ui;
};

#endif // MAP_WIDGET_H

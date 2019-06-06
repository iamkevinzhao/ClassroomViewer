#ifndef MAP_WIDGET_H
#define MAP_WIDGET_H

#include <QWidget>
#include <QVector2D>
#include <memory>
#include <QDebug>
#include <QPainter>

struct Room {
  QVector2D size;
};

struct Table {
  QVector2D size;
  QVector2D position;
  QVector2D heading;
  QString info() {
    return
        "size=(" +
            QString::number(size.x()) + ", " +
            QString::number(size.y()) + ") " +
        "position=(" +
            QString::number(position.x()) + ", " +
            QString::number(position.y()) + ") " +
        "heading=(" +
            QString::number(heading.x()) + ", " +
            QString::number(heading.y()) + ")";
  }
  QRect RectInRoom(const Room& room, const float& ratio) {
    return
        QRect(
            (position.x() - size.x() / 2) * ratio,
            (room.size.y() - position.y() - size.y()) * ratio,
            size.x() * ratio, size.y() * ratio);
  }
  void DrawProfileInRoom(
      QPainter& painter, const QImage& image,
      const Room& room, const float& ratio) {
    int image_size = size.y() * ratio * 0.9;
    painter.drawImage((position.x() + heading.x() * size.y() / 2) * ratio - image_size / 2, (room.size.y() - position.y() - heading.y() * size.y() / 2) * ratio - image_size / 2, image.scaled(image_size, image_size));
  }
};

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

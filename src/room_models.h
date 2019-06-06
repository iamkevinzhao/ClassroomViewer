#ifndef ROOM_MODELS_H
#define ROOM_MODELS_H

#include <QVector2D>
#include <QDebug>
#include <QPainter>
#include "profile.h"

struct Room {
  QVector2D size;
};

struct Table {
  QVector2D size;
  QVector2D position;
  QString owner = "Kevin";
  QString info() {
    return
        "size=(" +
            QString::number(size.x()) + ", " +
            QString::number(size.y()) + ") " +
        "position=(" +
            QString::number(position.x()) + ", " +
            QString::number(position.y()) + ")";
  }
  QRect RectInRoom(const Room& room, const float& ratio) {
    return
        QRect(
            (position.x() - size.x() / 2) * ratio,
            (room.size.y() - position.y() - size.y() / 2) * ratio,
            size.x() * ratio, size.y() * ratio);
  }
  void DrawProfileInRoom(
      QPainter& painter, const QImage& image,
      const Room& room, const float& ratio) {
    int image_size = (size.y() > size.x() ? size.x() : size.y()) * ratio * 0.9;
    painter.drawImage(
        (position.x()) * ratio - image_size / 2,
        (room.size.y() - position.y()) * ratio - image_size / 2,
        image.scaled(image_size, image_size));
  }
  void DrawProfileInRoom(
      QPainter &painter, const Room &room, const float &ratio) {
    if (!gFacebook) {
      return;
    }
    if (!gFacebook->count(owner)) {
      return;
    }
    DrawProfileInRoom(painter, gFacebook->value(owner).image, room, ratio);
  }
};

#endif // ROOM_MODELS_H

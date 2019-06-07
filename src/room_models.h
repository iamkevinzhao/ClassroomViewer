#ifndef ROOM_MODELS_H
#define ROOM_MODELS_H

#include <QVector2D>
#include <QDebug>
#include <QPainter>
#include "profile.h"

//struct Transform {
//  Transform(const Pose& from, const Pose& to) {
//    t = to.pos - from.pos;
//    float dot = to.ori.x() * from.ori.x() + to.ori.y() * from.ori.y();
//    float det = to.ori.x() * from.ori.y() - to.ori.y() * from.ori.x();
//    float angle = atan2(det, dot);
//    R.rotate(angle);
//  }
//  QVector2D t;
//  QMatrix R;
//};

struct Pose {
  QVector2D pos;
  QVector2D ori;
};

struct Room {
  QVector2D size;
};

struct Transform {
  Transform() {}
  Transform(const Pose& from, const Pose& to) {
    t = to.pos - from.pos;
    float dot = to.ori.x() * from.ori.x() + to.ori.y() * from.ori.y();
    float det = to.ori.x() * from.ori.y() - to.ori.y() * from.ori.x();
    float angle = atan2(det, dot) * 180 / M_PI;
    R.rotate(-angle);
  }
  Pose transform(const Pose& pose) const {
    Pose p = pose;
    p.pos = pose.pos + t;
    QPointF point = pose.ori.toPointF() * R;
    p.ori = QVector2D((float)point.x(), (float)point.y());
    return p;
  }
  float angle() const {
    return acos(R.m11()) * 180.0 / M_PI;
  }
  static QVector2D Rotate(const QVector2D& ori, const float& deg) {
    QMatrix R;
    R.rotate(deg);
    QPointF p = ori.toPointF() * R;
    return {(float)p.x(), (float)p.y()};
  }
  QVector2D Rotate(const QVector2D &ori) const {
    QPointF p = ori.toPointF() * R;
    return {(float)p.x(), (float)p.y()};
  }
  QVector2D t;
  QMatrix R;
};

struct Table {
  QVector2D size;
  QVector2D position;
  QString owner;
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

//inline QMatrix CalcTrans(const Pose& to, const Pose& from) {
//  QMatrix trans;
//  trans.translate((to.pos - from.pos).x(), (to.pos - from.pos).y());

//  float dot = to.ori.x() * from.ori.x() + to.ori.y() * from.ori.y();
//  float det = to.ori.x() * from.ori.y() - to.ori.y() * from.ori.x();
//  float angle = atan2(det, dot) * 180 / M_PI;
//  trans.rotate(-angle);
//  // trans.dx()
//  return trans;
//}

//inline QVector2D Translate(const QVector2D& pos, const QMatrix& trans) {
//  QVector2D new_pose;
//  new_pose = pos + QVector2D(trans.dx(), trans.dy());
//  return new_pose;
//}

//inline QVector2D Rotate(const QVector2D& ori, QMatrix trans) {
//  // QVector2D new_ori;
//  trans.setMatrix(trans.m11(), trans.m12(), trans.m21(), trans.m22(), 0, 0);
//  QPointF point = ori.toPointF() * trans;
//  return {point.x(), point.y()};
//}
#endif // ROOM_MODELS_H

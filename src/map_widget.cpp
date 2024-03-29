#include "map_widget.h"
#include "ui_map_widget.h"
#include <QtXml/QDomDocument>
#include <QFile>
#include <iostream>
#include <QDebug>
#include <QPainter>
#include "profile.h"

MapWidget::MapWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MapWidget)
{
  ui->setupUi(this);
  LoadFacebook();

  QDomDocument xml;
  QFile f("room.xml");
  if (!f.open(QIODevice::ReadOnly)) {
    std::cerr << "Error while loading xml file" << std::endl;
  }
  xml.setContent(&f);
  f.close();

  bool ok;
  QDomElement root = xml.documentElement();
  room.size.setX(root.attribute("LENGTH", "0.0").toFloat());
  room.size.setY(root.attribute("WIDTH", "0.0").toFloat());

  QDomElement table = root.firstChild().toElement();
  while (!table.isNull()) {
    if (table.tagName() == "TABLE") {
      Table tab;
      tab.size.setX(table.attribute("LENGTH", "0.0").toFloat());
      tab.size.setY(table.attribute("WIDTH", "0.0").toFloat());
      QDomElement position = table.firstChildElement("POSITION");
      if (position.isNull()) {
        continue;
      }
      tab.position.setX(position.attribute("X").toFloat());
      tab.position.setY(position.attribute("Y").toFloat());
      QDomElement heading = position.nextSiblingElement("HEADING");
      if (heading.isNull()) {
        continue;
      }
      tab.heading.setX(heading.attribute("X").toFloat());
      tab.heading.setY(heading.attribute("Y").toFloat());
      tables.push_back(tab);
      table = table.nextSibling().toElement();
    }
  }
}

MapWidget::~MapWidget()
{
  delete ui;
}

void MapWidget::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  if (room.size == QVector2D(0.0f, 0.0f)) {
    return;
  }
  const int fixed_width = this->size().width();
  const float ratio = fixed_width / room.size.x();
  painter.drawRect(0, 0, room.size.x() * ratio, room.size.y() * ratio);

  for (Table& table : tables) {
    if (table.size == QVector2D(0.0f, 0.0f)) {
      continue;
    }
    // DrawHeadingMarkInRoom({table.position, table.heading}, room, ratio, painter);
    painter.drawRect(table.RectInRoom(room, ratio));
    table.DrawProfileInRoom(painter, room, ratio);
  }

  // draw robot
  painter.setPen(Qt::blue);
  int robot_diam = fixed_width * 0.03;
  if (robot_diam < 5) {
    robot_diam = 5;
  }
  painter.drawEllipse(
      robot.pos.x() * ratio - robot_diam / 2.0f,
      (room.size.y() - robot.pos.y()) * ratio - robot_diam / 2.0f,
      robot_diam, robot_diam);
  float beacon_real = robot_diam / 2.0f * 1.3f / ratio;
  painter.drawLine(
      robot.pos.x() * ratio, (room.size.y() - robot.pos.y()) * ratio,
      (robot.pos.x() + robot.ori.x() * beacon_real) * ratio,
      (room.size.y() - robot.pos.y() - robot.ori.y() * beacon_real) * ratio);

  // calibration beacon
  painter.setPen(Qt::red);
#if 0
  DrawHeadingMarkInRoom(calib_pose, room, ratio, painter);
#else
  painter.drawLine(
      calib_pose.pos.x() * ratio, (room.size.y() - calib_pose.pos.y()) * ratio,
      (calib_pose.pos.x() + calib_pose.ori.x() * beacon_real) * ratio,
      (room.size.y() - calib_pose.pos.y() - calib_pose.ori.y() * beacon_real) * ratio);
  beacon_real *= 0.5;
  QVector2D perp = Transform::Rotate(calib_pose.ori, -90);
  painter.drawLine(
      calib_pose.pos.x() * ratio, (room.size.y() - calib_pose.pos.y()) * ratio,
      (calib_pose.pos.x() + perp.x() * beacon_real) * ratio,
      (room.size.y() - calib_pose.pos.y() - perp.y() * beacon_real) * ratio);
#endif
}

void MapWidget::DrawHeadingMarkInRoom(
    const Pose &pose, const Room& room, const float& ratio, QPainter& painter) {
  // painter.setPen(Qt::red);
  float beacon_real = room.size.x() * 0.01;
  painter.drawLine(
      pose.pos.x() * ratio, (room.size.y() - pose.pos.y()) * ratio,
      (pose.pos.x() + pose.ori.x() * beacon_real) * ratio,
      (room.size.y() - pose.pos.y() - pose.ori.y() * beacon_real) * ratio);
  beacon_real *= 0.5;
  QVector2D perp = Transform::Rotate(pose.ori, -90);
  painter.drawLine(
      pose.pos.x() * ratio, (room.size.y() - pose.pos.y()) * ratio,
      (pose.pos.x() + perp.x() * beacon_real) * ratio,
      (room.size.y() - pose.pos.y() - perp.y() * beacon_real) * ratio);
}

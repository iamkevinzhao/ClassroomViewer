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
  if (!gFacebook) {
    gFacebook.reset(new Facebook);
  }
  gFacebook->insert("Kevin", Profile{"Kevin", QImage("kevin.png")});

  QDomDocument xml;
  QFile f("room.xml");
  if (!f.open(QIODevice::ReadOnly)) {
    std::cerr << "Error while loading xml file" << std::endl;
  }
  xml.setContent(&f);
  f.close();

  bool ok;
  QDomElement root = xml.documentElement();
  room_.size.setX(root.attribute("LENGTH", "0.0").toFloat());
  room_.size.setY(root.attribute("WIDTH", "0.0").toFloat());

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
  if (room_.size == QVector2D(0.0f, 0.0f)) {
    return;
  }
  const int fixed_width = this->size().width();
  const float ratio = fixed_width / room_.size.x();
  painter.drawRect(0, 0, room_.size.x() * ratio, room_.size.y() * ratio);

  for (Table& table : tables) {
    if (table.size == QVector2D(0.0f, 0.0f)) {
      continue;
    }
    painter.drawRect(table.RectInRoom(room_, ratio));
    // table.DrawProfileInRoom(painter, QImage("kevin.png"), room_, ratio);
    table.DrawProfileInRoom(painter, room_, ratio);
  }

  // draw robot
  painter.setPen(Qt::blue);
  int robot_diam = fixed_width * 0.03;
  if (robot_diam < 5) {
    robot_diam = 5;
  }
  painter.drawEllipse(
      robot.pos.x() * ratio - robot_diam / 2.0f,
      (room_.size.y() - robot.pos.y()) * ratio - robot_diam / 2.0f,
      robot_diam, robot_diam);
  float beacon_real = robot_diam / 2.0f * 1.3f / ratio;
  painter.drawLine(
      robot.pos.x() * ratio, (room_.size.y() - robot.pos.y()) * ratio,
      (robot.pos.x() + robot.ori.x() * beacon_real) * ratio,
      (room_.size.y() - robot.pos.y() - robot.ori.y() * beacon_real) * ratio);
}

#include "map_widget.h"
#include "ui_map_widget.h"
#include <QtXml/QDomDocument>
#include <QFile>
#include <iostream>
#include <QDebug>
#include <QPainter>

MapWidget::MapWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::MapWidget)
{
  ui->setupUi(this);
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
      QDomElement orientation = table.firstChildElement("ORIENTATION");
      if (orientation.isNull()) {
        continue;
      }
      tab.position.setX(position.attribute("X").toFloat());
      tab.position.setY(position.attribute("Y").toFloat());
      tab.heading.setX(orientation.attribute("X").toFloat());
      tab.heading.setY(orientation.attribute("Y").toFloat());
      tables_.push_back(tab);
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
  const int fixed_width = 800;
  const float ratio = fixed_width / room_.size.x();
  painter.drawRect(0, 0, room_.size.x() * ratio, room_.size.y() * ratio);

  for (Table& table : tables_) {
    if (table.size == QVector2D(0.0f, 0.0f)) {
      continue;
    }
    painter.drawRect(table.RectInRoom(room_, ratio));
    // painter.drawImage(QPoint(100, 100), QImage("soojin.jpeg").scaled(50, 50));
    table.DrawProfileInRoom(painter, QImage("kevin.png"), room_, ratio);
  }
}

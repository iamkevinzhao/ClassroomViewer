#include "server.h"
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>

QString Packet::info() const {
  return
      "ts:" + QString::number(ts) + " " +
      "pos:" + QString::number(pos.x()) + "," + QString::number(pos.y()) + " " +
      "ori:" + QString::number(ori.x()) + "," + QString::number(ori.y()) + " " +
      "dist:" + QString::number(dist);
}

bool Packet::Parse(QByteArray stream) {
  int index;
  bool ok;
  QByteArray tmp;
  float f;

  if (stream.left(3) != "ts:") {
    return false;
  }
  stream.remove(0, 3);
  index = stream.indexOf('|');
  if (index == -1) {
    return false;
  }
  ts = stream.left(index).toLong(&ok);
  if (!ok) {
    return false;
  }
  stream.remove(0, index + 1);

  // name
  if (stream.left(5) != "name:") {
    return false;
  }
  stream.remove(0, 5);
  index = stream.indexOf('|');
  if (index == -1) {
    return false;
  }
  name = stream.left(index);
  stream.remove(0, index + 1);

  // position
  if (stream.left(4) != "pos:") {
    return false;
  }
  stream.remove(0, 4);
  index = stream.indexOf('|');
  if (index == -1) {
    return false;
  }
  tmp = stream.left(index);
  stream.remove(0, index + 1);
  index = tmp.indexOf(',');
  if (index == -1) {
    return false;
  }
  f = tmp.left(index).toFloat(&ok);
  if (!ok) {
    return false;
  }
  pos.setX(f);
  tmp.remove(0, index + 1);
  f = tmp.toFloat(&ok);
  if (!ok) {
    return false;
  }
  pos.setY(f);

  // orientation
  if (stream.left(4) != "ori:") {
    return false;
  }
  stream.remove(0, 4);
  index = stream.indexOf('|');
  if (index == -1) {
    return false;
  }
  tmp = stream.left(index);
  stream.remove(0, index + 1);
  index = tmp.indexOf(',');
  if (index == -1) {
    return false;
  }
  f = tmp.left(index).toFloat(&ok);
  if (!ok) {
    return false;
  }
  ori.setX(f);
  tmp.remove(0, index + 1);
  f = tmp.toFloat(&ok);
  if (!ok) {
    return false;
  }
  ori.setY(f);
  ori.normalize();

  // distance
  if (stream.left(5) != "dist:") {
    return false;
  }
  stream.remove(0, 5);
  dist = stream.toFloat(&ok);
  if (!ok) {
    return false;
  }
  return true;
}

void Packet::Parse(QByteArray stream, std::vector<Packet> &packets) {
  int left_index = stream.lastIndexOf(']');
  if (left_index == -1) {
    return;
  }
  stream.remove(left_index, stream.size());
  int right_index = stream.lastIndexOf('[');
  if (right_index == -1) {
    return;
  }
  QByteArray packet_array = stream.right(stream.size() - right_index - 1);
  stream.remove(right_index, stream.size() - right_index);
  Packet packet;
  if (packet.Parse(packet_array)) {
    packets.push_back(packet);
  }
  Parse(stream, packets);
}

Server::Server()
{
  listen(QHostAddress::Any, 2369);
  connect(this, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));
//  std::vector<Packet> packets;
//  Packet::Parse("[ts:13413241234|name:Kevin|pos:10.0,4.5|ori:4.0,2.3|dist:4.0]", packets);
}

void Server::OnNewConnection() {
  QTcpSocket* client = nextPendingConnection();
  connect(client, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
  connect(
      client, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
      this, SLOT(OnSocketStateChanged(QAbstractSocket::SocketState)));
  sockets_.push_back(client);
}

void Server::OnReadyRead() {
  QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
  QByteArray data = sender->readAll();
  std::vector<Packet> pkts;
  Packet::Parse(data, pkts);
  packets.insert(packets.begin(), pkts.begin(), pkts.end());
  constexpr int buf_size = 10000;
  if (packets.size() > buf_size) {
    packets.erase(packets.end() - (packets.size() - buf_size), packets.end());
  }
  Process();
}

void Server::OnSocketStateChanged(QAbstractSocket::SocketState socketState)
{
  if (socketState == QAbstractSocket::UnconnectedState)
  {
    QTcpSocket* sender = static_cast<QTcpSocket*>(QObject::sender());
    sockets_.removeOne(sender);
  }
}


//////////////////////////// Map Server ///////////////////////////
MapServer::MapServer() {}

void MapServer::Process() {
  if (!map) {
    return;
  }
  const QString kRobot = "robot";
  for (Packet& packet : packets) {
    if (packet.name == kRobot) {
      newest_pose_.pos = packet.pos;
      newest_pose_.ori = packet.ori;
      break;
    }
  }
  for (Packet& packet : packets) {
    packet.ori = calib_trans_.Rotate(packet.ori);
    packet.pos = packet.pos + calib_trans_.t;
  }
  // int tab_id = 0;
  for (Packet& packet : packets) {
    if (packet.name == kRobot) {
      continue;
    }
    Table* closest = FindClosestTable(packet, map->tables);
    if (!closest) {
      continue;
    }
    closest->owner = packet.name;
    for (Table& table : map->tables) {
      if (&table == closest) {
        continue;
      }
      if (table.owner == packet.name) {
        table.owner = "";
      }
    }
//    for (Table& table : map->tables) {
//      // if (table.position.distanceToPoint(packet.pos) < packet.dist) {
////      if (table.position.distanceToPoint(packet.pos) < 1.5) {
//      if (IsTheClosestTable(table, map->tables, packet)) {
//        table.owner = packet.name;
//        for (int i = 0; i < map->tables.size(); ++i) {
//          if (i == tab_id) {
//            continue;
//          }
//          if (map->tables[i].owner == packet.name) {
//            map->tables[i].owner = "";
//          }
//        }
//      }
//      ++tab_id;
//    }
  }
  for (Packet& packet : packets) {
    if (packet.name == kRobot) {
      map->robot.pos = packet.pos;
      map->robot.ori = packet.ori;
      break;
    }
  }
  map->update();
  packets.clear();
}

void MapServer::OnCalibrated(Pose pose) {
  calib_trans_ = Transform(newest_pose_, pose);
  if (map) {
    map->robot = calib_trans_.transform(newest_pose_);
    map->update();
  }
}

Table* MapServer::FindClosestTable(
    const Packet &packet, std::vector<Table> &tables) {
  float max_dist = 1000.0;
  Table* result = nullptr;
  for (Table& table : tables) {
    float query_range = 0.5;
    float table_range = 1.0;
    QVector2D query_end = packet.pos + packet.ori * query_range;
    QVector2D seat_end = table.position + table.heading * table_range;
    float dist = (query_end - seat_end).length();
    if (dist < max_dist) {
      max_dist = dist;
      result = &table;
    }
  }
  if (max_dist > 1.0f) {
    result = nullptr;
  }
  return result;
}

bool MapServer::IsTheClosestTable(const Table& query, const std::vector<Table>& tables,
    const Packet& packet) {
  float query_range = 0.5;
  float table_range = 1.0;
  QVector2D query_end = packet.pos + packet.ori * query_range;
  QVector2D seat_end = query.position + query.heading * table_range;
  float query_dist = (query_end - seat_end).length();
  // const Table* best = nullptr;
  for (const Table& table : tables) {
    query_end = packet.pos + packet.ori * query_range;
    seat_end = table.position + table.heading * table_range;
    float dist = (query_end - seat_end).length();
    if (dist < query_dist) {
      false;
    }
  }
  return true;
}

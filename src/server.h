#ifndef RECEIVER_H
#define RECEIVER_H

#include <QTcpServer>
#include <QVector2D>
#include "map_widget.h"

struct Packet {
  // [name|position|orientation|dist]
  QString name;
  QVector2D pos;
  QVector2D ori;
  float dist;
  long ts;

  static void Parse(QByteArray stream, std::vector<Packet>& packets);
  bool Parse(QByteArray stream);
  QString info();
};

class Server : public QTcpServer
{
  Q_OBJECT
public:
  Server();
  std::vector<Packet> packets;
protected:
  virtual void Process() = 0;
public slots:
  void OnNewConnection();
  void OnReadyRead();
  void OnSocketStateChanged(QAbstractSocket::SocketState socketState);
private:
  QList<QTcpSocket*>  sockets_;
};

class MapServer : public Server {
  Q_OBJECT
public:
  MapServer();
  MapWidget* map = nullptr;
public slots:
  void OnCalibrated(Pose pose);
protected:
  void Process() override;
private:
  Pose newest_pose_;
  Transform calib_trans_;
  // Transform cali_trans_;
};

#endif // RECEIVER_H

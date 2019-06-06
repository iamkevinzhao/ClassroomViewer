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
  MapWidget* map = nullptr;
protected:
  void Process();
public slots:
  void OnNewConnection();
  void OnReadyRead();
  void OnSocketStateChanged(QAbstractSocket::SocketState socketState);
private:
  QList<QTcpSocket*>  sockets_;
};

#endif // RECEIVER_H

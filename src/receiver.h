#ifndef RECEIVER_H
#define RECEIVER_H

#include <QTcpServer>
#include <QVector2D>

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

class Receiver : public QTcpServer
{
  Q_OBJECT
public:
  Receiver();
  std::vector<Packet> packets;
public slots:
  void OnNewConnection();
  void OnReadyRead();
  void OnSocketStateChanged(QAbstractSocket::SocketState socketState);
private:
  QList<QTcpSocket*>  sockets_;
};

#endif // RECEIVER_H

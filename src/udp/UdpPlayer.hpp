#pragma once

#include <QHostAddress>
#include <QReadWriteLock>
#include <QThread>

class QFile;
class QElapsedTimer;
class QUdpSocket;

class UdpPlayer : public QThread {
public:
  explicit UdpPlayer(QObject *parent = nullptr);
  virtual ~UdpPlayer() override;

  void start();
  void stop();
  void restart();

  qint64 getCurrentTime() const;

  void setPort(int port);
  void setHostAddress(const QHostAddress &address);
  void setFileName(const QString &fileName);

protected:
  virtual void run() override;

private:
  mutable QReadWriteLock mLock;

  QScopedPointer<QUdpSocket> mUdpSocket;
  QScopedPointer<QFile> mFile;
  QScopedPointer<QDataStream> mDataStream;
  QScopedPointer<QElapsedTimer> mTimer;

  QHostAddress mHostAddress;
  int mPort = 6666;
  QString mFileName;
};

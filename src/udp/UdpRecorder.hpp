#pragma once

#include <QAbstractSocket>
#include <QHostAddress>
#include <QReadWriteLock>
#include <QThread>

class QFile;
class QElapsedTimer;
class QUdpSocket;

class UdpRecorder : public QThread {
public:
  explicit UdpRecorder(QObject *parent = nullptr);
  virtual ~UdpRecorder() override;

  void start();
  void stop();
  void restart();

  qint64 getCurrentTime() const;
  qint64 getCurrentFileSize() const;

  void setPort(int port);
  void setHostAddress(const QHostAddress &address);
  void setFileName(const QString &fileName);

protected slots:
  void showError(QAbstractSocket::SocketError error);

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

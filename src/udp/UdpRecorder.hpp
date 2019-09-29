#pragma once

#include <QReadWriteLock>
#include <QSharedPointer>
#include <QThread>
#include <QUdpSocket>

class QFile;
class QElapsedTimer;

class UdpRecorder : public QThread {
public:
  explicit UdpRecorder(QObject *parent = nullptr);
  virtual ~UdpRecorder() override;

  void start();
  void stop();
  void restart();

  qint64 getCurrentTime() const;

  void setPort(int port);
  void setFileName(const QString &fileName);

protected slots:
  void readData();
  void showError(QAbstractSocket::SocketError error);

protected:
  virtual void run() override;

private:
  mutable QReadWriteLock mLock;

  QSharedPointer<QUdpSocket> mUdpSocket;
  QSharedPointer<QFile> mFile;
  QSharedPointer<QDataStream> mDataStream;
  QSharedPointer<QElapsedTimer> mTimer;

  int mPort = 6666;
  QString mFileName;
};

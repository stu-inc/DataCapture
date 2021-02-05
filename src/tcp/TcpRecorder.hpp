#pragma once

#include <QHostAddress>
#include <QReadWriteLock>
#include <QSharedPointer>
#include <QTcpSocket>
#include <QThread>

class QFile;
class QElapsedTimer;

class TcpRecorder : public QThread {
public:
  explicit TcpRecorder(QObject *parent = nullptr);
  virtual ~TcpRecorder() override;

  void start();
  void stop();
  void restart();

  qint64 getCurrentTime() const;
  qint64 getCurrentFileSize() const;

  void setPort(int port);
  void setHostAddress(const QHostAddress &address);
  void setFileName(const QString &fileName);

protected slots:
  void readData();
  void showError(QAbstractSocket::SocketError error);

protected:
  virtual void run() override;

private:
  mutable QReadWriteLock mLock;

  QSharedPointer<QTcpSocket> mTcpSocket;
  QSharedPointer<QFile> mFile;
  QSharedPointer<QDataStream> mDataStream;
  QSharedPointer<QElapsedTimer> mTimer;

  QHostAddress mHostAddress;
  int mPort = 6666;
  QString mFileName;
};

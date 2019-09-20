#pragma once

#include <QReadWriteLock>
#include <QSharedPointer>
#include <QThread>

class QSerialPort;
class QFile;
class QElapsedTimer;

class SerialPlayer : public QThread {
public:
  explicit SerialPlayer(QObject *parent = nullptr);
  virtual ~SerialPlayer() override;

  void start();
  void stop();
  void restart();

  virtual void run() override;

  void setPortName(const QString &portName);
  void setFileName(const QString &fileName);

private:
  mutable QReadWriteLock mLock;

  QSharedPointer<QSerialPort> mSerialPort;
  QSharedPointer<QFile> mFile;
  QSharedPointer<QDataStream> mDataStream;
  QSharedPointer<QElapsedTimer> mTimer;

  QString mPortName;
  QString mFileName;
};

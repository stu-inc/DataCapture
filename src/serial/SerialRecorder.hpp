#pragma once

#include <QReadWriteLock>
#include <QSharedPointer>
#include <QThread>

class QSerialPort;
class QFile;
class QElapsedTimer;

class SerialRecorder : public QThread {
public:
  explicit SerialRecorder(QObject *parent = nullptr);
  virtual ~SerialRecorder() override;

  void start();
  void stop();
  void restart();

  virtual void run() override;

  void setPortName(const QString &portName);
  void setFileName(const QString &fileName);

protected slots:
  void readData();

private:
  mutable QReadWriteLock mLock;

  QSharedPointer<QSerialPort> mSerialPort;
  QSharedPointer<QFile> mFile;
  QSharedPointer<QDataStream> mDataStream;
  QSharedPointer<QElapsedTimer> mTimer;

  QString mPortName;
  QString mFileName;
};

#pragma once

#include <QReadWriteLock>
#include <QSharedPointer>
#include <QThread>

class QSerialPort;

class SerialRecorder : public QThread {
public:
  explicit SerialRecorder(QObject *parent = nullptr);
  virtual ~SerialRecorder() override;

  void start();
  void stop();
  void restart();

  virtual void run() override;

  void setPortName(const QString &name);

protected slots:
  void readData();

private:
  mutable QReadWriteLock mLock;

  QSharedPointer<QSerialPort> mSerialPort;

  QString mPortName;
};

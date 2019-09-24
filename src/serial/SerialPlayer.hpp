#pragma once

#include <QReadWriteLock>
#include <QSharedPointer>
#include <QThread>
#include <QtSerialPort>

class QFile;
class QElapsedTimer;

class SerialPlayer : public QThread {
public:
  explicit SerialPlayer(QObject *parent = nullptr);
  virtual ~SerialPlayer() override;

  void start();
  void stop();
  void restart();

  qint64 getCurrentTime() const;

  void setPortName(const QString &portName);
  void setFileName(const QString &fileName);

  void setBaundRate(QSerialPort::BaudRate baudRate);
  void setDataBits(QSerialPort::DataBits dataBits);
  void setParity(QSerialPort::Parity parity);
  void setStopBits(QSerialPort::StopBits stopBits);

protected:
  virtual void run() override;

private:
  mutable QReadWriteLock mLock;

  QSharedPointer<QSerialPort> mSerialPort;
  QSharedPointer<QFile> mFile;
  QSharedPointer<QDataStream> mDataStream;
  QSharedPointer<QElapsedTimer> mTimer;

  QString mPortName;
  QString mFileName;

  QSerialPort::BaudRate mBaundRate;
  QSerialPort::DataBits mDataBits;
  QSerialPort::Parity mParity;
  QSerialPort::StopBits mStopBits;
};

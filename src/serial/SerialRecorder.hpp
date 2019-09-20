#pragma once

#include <QReadWriteLock>
#include <QSerialPort>
#include <QSharedPointer>
#include <QThread>

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

  void setBaundRate(QSerialPort::BaudRate baudRate);
  void setDataBits(QSerialPort::DataBits dataBits);
  void setParity(QSerialPort::Parity parity);
  void setStopBits(QSerialPort::StopBits stopBits);

protected slots:
  void readData();
  void showError(QSerialPort::SerialPortError error);

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

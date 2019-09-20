#include "SerialRecorder.hpp"
#include <QDebug>
#include <QSerialPort>

SerialRecorder::SerialRecorder(QObject *parent) : QThread(parent) {}

SerialRecorder::~SerialRecorder() { stop(); }

void SerialRecorder::start() { QThread::start(); }

void SerialRecorder::stop() {
  requestInterruption();
  wait();
}

void SerialRecorder::restart() {
  stop();
  start();
}

void SerialRecorder::setPortName(const QString &name) {
  QWriteLocker locker(&mLock);
  mPortName = name;
}

void SerialRecorder::readData() {
  auto bytes = mSerialPort->readAll();
  qDebug() << bytes;
}

void SerialRecorder::run() {

  mSerialPort = QSharedPointer<QSerialPort>::create();

  {
    QReadLocker locker(&mLock);
    mSerialPort->setPortName(mPortName);
  }

  if (!mSerialPort->open(QIODevice::ReadOnly)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open serial port: " + mPortName;
    return;
  }

  connect(mSerialPort.data(), &QSerialPort::readyRead, this,
          &SerialRecorder::readData);

  exec();

  mSerialPort->close();
  mSerialPort.clear();
}

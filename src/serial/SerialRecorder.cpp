#include "SerialRecorder.hpp"
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>

SerialRecorder::SerialRecorder(QObject *parent) : QThread(parent) {}

SerialRecorder::~SerialRecorder() { stop(); }

void SerialRecorder::start() { QThread::start(); }

void SerialRecorder::stop() {
  quit();
  wait();
}

void SerialRecorder::restart() {
  stop();
  start();
}

void SerialRecorder::setPortName(const QString &portName) {
  QWriteLocker locker(&mLock);
  mPortName = portName;
}

void SerialRecorder::setFileName(const QString &fileName) {
  QWriteLocker locker(&mLock);
  mFileName = fileName;
}

void SerialRecorder::setBaundRate(QSerialPort::BaudRate baudRate) {
  QWriteLocker locker(&mLock);
  mBaundRate = baudRate;
}

void SerialRecorder::setDataBits(QSerialPort::DataBits dataBits) {
  QWriteLocker locker(&mLock);
  mDataBits = dataBits;
}

void SerialRecorder::setParity(QSerialPort::Parity parity) {
  QWriteLocker locker(&mLock);
  mParity = parity;
}

void SerialRecorder::setStopBits(QSerialPort::StopBits stopBits) {
  QWriteLocker locker(&mLock);
  mStopBits = stopBits;
}

void SerialRecorder::readData() {
  auto bytes = mSerialPort->readAll();
  if (mTimer->isValid()) {
    *mDataStream << mTimer->elapsed();
    *mDataStream << bytes;
  }
}

void SerialRecorder::showError(QSerialPort::SerialPortError error) {
  qWarning() << error;
}

void SerialRecorder::run() {

  mFile = QSharedPointer<QFile>::create();
  mDataStream = QSharedPointer<QDataStream>::create();
  mSerialPort = QSharedPointer<QSerialPort>::create();
  mTimer = QSharedPointer<QElapsedTimer>::create();

  {
    QReadLocker locker(&mLock);
    mFile->setFileName(mFileName);
  }

  if (!mFile->open(QFile::WriteOnly)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open file: " + mFileName;
    return;
  }

  mDataStream->setDevice(mFile.data());

  {
    QReadLocker locker(&mLock);
    mSerialPort->setPortName(mPortName);
    mSerialPort->setBaudRate(mBaundRate);
    mSerialPort->setDataBits(mDataBits);
    mSerialPort->setParity(mParity);
    mSerialPort->setStopBits(mStopBits);
  }

  connect(mSerialPort.data(), &QSerialPort::errorOccurred, this,
          &SerialRecorder::showError);

  connect(mSerialPort.data(), &QSerialPort::readyRead, this,
          &SerialRecorder::readData);

  if (!mSerialPort->open(QIODevice::ReadWrite)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open serial port: " + mPortName
               << mSerialPort->error();
    return;
  }

  mTimer->start();

  exec();

  mTimer->invalidate();

  mSerialPort->close();
  mFile->close();

  mSerialPort.clear();
  mDataStream.clear();
  mFile.clear();
  mTimer.clear();
}

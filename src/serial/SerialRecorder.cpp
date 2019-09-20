#include "SerialRecorder.hpp"
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
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

void SerialRecorder::setPortName(const QString &portName) {
  QWriteLocker locker(&mLock);
  mPortName = portName;
}

void SerialRecorder::setFileName(const QString &fileName) {
  QWriteLocker locker(&mLock);
  mFileName = fileName;
}

void SerialRecorder::readData() {
  auto bytes = mSerialPort->readAll();
  if (mTimer->isValid()) {
    *mDataStream << mTimer->elapsed();
    *mDataStream << bytes;
  }
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

  {
    QReadLocker locker(&mLock);
    mSerialPort->setPortName(mPortName);
  }

  if (!mSerialPort->open(QIODevice::ReadOnly)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open serial port: " + mPortName;
    return;
  }

  mDataStream->setDevice(mFile.data());

  connect(mSerialPort.data(), &QSerialPort::readyRead, this,
          &SerialRecorder::readData);

  mTimer->start();

  exec();

  mTimer->invalidate();

  mSerialPort->close();
  mFile->close();

  mSerialPort.clear();
  mDataStream.clear();
  mFile.clear();
}

#include "SerialPlayer.hpp"
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

SerialPlayer::SerialPlayer(QObject *parent) : QThread(parent) {}

SerialPlayer::~SerialPlayer() { stop(); }

void SerialPlayer::start() { QThread::start(); }

void SerialPlayer::stop() {
  requestInterruption();
  wait();
}

void SerialPlayer::restart() {
  stop();
  start();
}

qint64 SerialPlayer::getCurrentTime() const {
  QReadLocker locker(&mLock);
  return mTimer.isNull() || !mTimer->isValid() ? 0 : mTimer->elapsed();
}

void SerialPlayer::setPortName(const QString &name) {
  QWriteLocker locker(&mLock);
  mPortName = name;
}

void SerialPlayer::setFileName(const QString &fileName) {
  QWriteLocker locker(&mLock);
  mFileName = fileName;
}

void SerialPlayer::setBaundRate(QSerialPort::BaudRate baudRate) {
  QWriteLocker locker(&mLock);
  mBaundRate = baudRate;
}

void SerialPlayer::setDataBits(QSerialPort::DataBits dataBits) {
  QWriteLocker locker(&mLock);
  mDataBits = dataBits;
}

void SerialPlayer::setParity(QSerialPort::Parity parity) {
  QWriteLocker locker(&mLock);
  mParity = parity;
}

void SerialPlayer::setStopBits(QSerialPort::StopBits stopBits) {
  QWriteLocker locker(&mLock);
  mStopBits = stopBits;
}

void SerialPlayer::run() {

  mFile = QSharedPointer<QFile>::create();
  mDataStream = QSharedPointer<QDataStream>::create();
  mSerialPort = QSharedPointer<QSerialPort>::create();
  mTimer = QSharedPointer<QElapsedTimer>::create();

  {
    QReadLocker locker(&mLock);
    mFile->setFileName(mFileName);
  }

  if (!mFile->open(QFile::ReadOnly)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open file: " + mFileName;
    return;
  }

  {
    QReadLocker locker(&mLock);
    mSerialPort->setPortName(mPortName);
    mSerialPort->setBaudRate(mBaundRate);
    mSerialPort->setDataBits(mDataBits);
    mSerialPort->setParity(mParity);
    mSerialPort->setStopBits(mStopBits);
  }

  if (!mSerialPort->open(QIODevice::WriteOnly)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open serial port: " + mPortName;
    return;
  }

  mDataStream->setDevice(mFile.data());

  auto info = QJsonDocument::fromBinaryData(mFile->read(1000)).object();

  int fileSize = info["FileSize"].toInt();
  int timeLength = info["TimeLength"].toInt();

  mTimer->start();

  while (!isInterruptionRequested()) {

    qint64 time;
    QByteArray bytes;

    *mDataStream >> time;
    *mDataStream >> bytes;

    while (true) {
      if (mTimer->elapsed() >= time) {
        mSerialPort->write(bytes);
        mSerialPort->flush();
        break;
      }
      msleep(10);
    }

    if (mDataStream->atEnd())
      break;
  }

  mTimer->invalidate();

  mSerialPort->close();
  mFile->close();

  mSerialPort.clear();
  mDataStream.clear();
  mFile.clear();
  mTimer.clear();
}

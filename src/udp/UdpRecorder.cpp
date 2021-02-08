#include "UdpRecorder.hpp"
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkDatagram>
#include <QUdpSocket>

UdpRecorder::UdpRecorder(QObject *parent) : QThread(parent) {}

UdpRecorder::~UdpRecorder() { stop(); }

void UdpRecorder::start() { QThread::start(); }

void UdpRecorder::stop() {
  requestInterruption();
  wait();
}

void UdpRecorder::restart() {
  stop();
  start();
}

qint64 UdpRecorder::getCurrentTime() const {
  QReadLocker locker(&mLock);
  return mTimer.isNull() || !mTimer->isValid() ? 0 : mTimer->elapsed();
}

qint64 UdpRecorder::getCurrentFileSize() const {
  QReadLocker locker(&mLock);
  return mFile.isNull() ? 0 : mFile->size() - 1000;
}

void UdpRecorder::setPort(int port) {
  QWriteLocker locker(&mLock);
  mPort = port;
}

void UdpRecorder::setHostAddress(const QHostAddress &address) {
  QWriteLocker locker(&mLock);
  mHostAddress = address;
}

void UdpRecorder::setFileName(const QString &fileName) {
  QWriteLocker locker(&mLock);
  mFileName = fileName;
}

void UdpRecorder::showError(QAbstractSocket::SocketError error) {
  qWarning() << error;
}

void UdpRecorder::run() {

  mFile.reset(new QFile);
  mDataStream.reset(new QDataStream);
  mUdpSocket.reset(new QUdpSocket);
  mTimer.reset(new QElapsedTimer);

  {
    QReadLocker locker(&mLock);
    mFile->setFileName(mFileName);
  }

  if (!mFile->open(QFile::WriteOnly)) {
    QReadLocker locker(&mLock);
    qWarning() << "Can not open file: " + mFileName;
    return;
  }

  // FIXME: why can not connect this signal?
  // connect(mUdpSocket.data(), &QUdpSocket::error, this,
  // &UdpRecorder::showError);

  {
    QReadLocker locker(&mLock);
    if (!mUdpSocket->bind(mHostAddress, quint16(mPort))) {
      qWarning() << "Can not bind udp server: " + QString::number(mPort)
                 << mUdpSocket->error();
      return;
    }
  }

  // Fill up data info segments with zero
  mFile->write(QByteArray(1000, '0'));

  mDataStream->setDevice(mFile.data());

  mTimer->start();

  while (!isInterruptionRequested()) {

    if (mUdpSocket->waitForReadyRead(100)) {

      while (mUdpSocket->hasPendingDatagrams()) {

        auto data = mUdpSocket->receiveDatagram().data();

        QWriteLocker locker(&mLock);

        if (mTimer->isValid()) {
          *mDataStream << mTimer->elapsed();
          *mDataStream << data;
        }
      }
    }
  }

  auto time = mTimer->elapsed();
  mTimer->invalidate();

  // Data info
  QJsonObject info;

  info["FileSize"] = mFile->size();
  info["TimeLength"] = time;

  mFile->seek(0);
  mFile->write(QJsonDocument(info).toBinaryData());

  mUdpSocket->close();
  mFile->close();

  mUdpSocket.reset();
  mDataStream.reset();
  mFile.reset();
  mTimer.reset();
}

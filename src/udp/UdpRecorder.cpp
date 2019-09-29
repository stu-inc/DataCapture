#include "UdpRecorder.hpp"
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

UdpRecorder::UdpRecorder(QObject *parent) : QThread(parent) {}

UdpRecorder::~UdpRecorder() { stop(); }

void UdpRecorder::start() { QThread::start(); }

void UdpRecorder::stop() {
  quit();
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

void UdpRecorder::setPort(int port) {
  QWriteLocker locker(&mLock);
  mPort = port;
}

void UdpRecorder::setFileName(const QString &fileName) {
  QWriteLocker locker(&mLock);
  mFileName = fileName;
}

void UdpRecorder::readData() {
  QByteArray datagram;
  datagram.resize(int(mUdpSocket->pendingDatagramSize()));
  mUdpSocket->readDatagram(datagram.data(), datagram.size());
  if (mTimer->isValid()) {
    *mDataStream << mTimer->elapsed();
    *mDataStream << datagram;
    qDebug() << datagram;
  }
}

void UdpRecorder::showError(QAbstractSocket::SocketError error) {
  qWarning() << error;
}

void UdpRecorder::run() {

  mFile = QSharedPointer<QFile>::create();
  mDataStream = QSharedPointer<QDataStream>::create();
  mUdpSocket = QSharedPointer<QUdpSocket>::create();
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

  // connect(mUdpSocket.data(), &QUdpSocket::error, this,
  // &UdpRecorder::showError);

  connect(mUdpSocket.data(), &QUdpSocket::readyRead, this,
          &UdpRecorder::readData);

  {
    QReadLocker locker(&mLock);
    if (!mUdpSocket->bind(QHostAddress::Any, quint16(mPort))) {
      qWarning() << "Can not bind udp server: " + QString::number(mPort)
                 << mUdpSocket->error();
      return;
    }
  }

  // Fill up data info segments with zero
  mFile->write(QByteArray(1000, '0'));

  mDataStream->setDevice(mFile.data());

  mTimer->start();

  exec();

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

  mUdpSocket.clear();
  mDataStream.clear();
  mFile.clear();
  mTimer.clear();
}

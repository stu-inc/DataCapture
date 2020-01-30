#include "PlaybackView.hpp"
#include "serial/SerialPlayer.hpp"
#include "ui_playbackview.h"

PlaybackView::PlaybackView(QWidget *parent)
    : QWidget(parent), ui(new Ui::PlaybackView) {
  ui->setupUi(this);
  ui->pushButtonStopPlayback->setEnabled(false);
  mPlayer = QSharedPointer<SerialPlayer>::create();
}

PlaybackView::~PlaybackView() { delete ui; }

void PlaybackView::startPlayback() {

  ui->pushButtonStartPlayback->setEnabled(false);
  ui->groupBoxDevice->setEnabled(false);
  ui->groupBoxFile->setEnabled(false);
  ui->pushButtonStopPlayback->setEnabled(true);

  mPlayer->setPortName(ui->widgetSerialDevice->getCurrentDevice());
  mPlayer->setFileName(ui->widgetPlaybackFile->getFileName());
  mPlayer->setBaundRate(ui->widgetSerialDevice->getBaundRate());
  mPlayer->setDataBits(ui->widgetSerialDevice->getDataBits());
  mPlayer->setParity(ui->widgetSerialDevice->getParity());
  mPlayer->setStopBits(ui->widgetSerialDevice->getStopBits());
  mPlayer->setByteOrder(ui->widgetSerialDevice->getByteOrder());

  mPlayer->start();
  mTimerId = startTimer(33);
}

void PlaybackView::stopPlayback() {

  ui->pushButtonStopPlayback->setEnabled(false);
  ui->pushButtonStartPlayback->setEnabled(true);
  ui->groupBoxDevice->setEnabled(true);
  ui->groupBoxFile->setEnabled(true);

  mPlayer->stop();
  killTimer(mTimerId);
}

void PlaybackView::timerEvent(QTimerEvent *e) {
  QTime time(0, 0);
  auto current = time.addMSecs(int(mPlayer->getCurrentTime()));
  ui->widgetPlaybackFile->setTimeText(current.toString("HH:mm:ss:zzz"));
  e->accept();
}

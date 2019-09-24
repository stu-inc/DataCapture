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

  /*mRecorder->setPortName(ui->widgetSerialDevice->getCurrentDevice());
  mRecorder->setFileName(ui->widgetRecordFile->getFileName());

  mRecorder->setBaundRate(ui->widgetSerialDevice->getBaundRate());
  mRecorder->setDataBits(ui->widgetSerialDevice->getDataBits());
  mRecorder->setParity(ui->widgetSerialDevice->getParity());
  mRecorder->setStopBits(ui->widgetSerialDevice->getStopBits());*/

  mPlayer->start();
}

void PlaybackView::stopPlayback() {

  ui->pushButtonStopPlayback->setEnabled(false);
  ui->pushButtonStartPlayback->setEnabled(true);
  ui->groupBoxDevice->setEnabled(true);
  ui->groupBoxFile->setEnabled(true);

  mPlayer->stop();
}

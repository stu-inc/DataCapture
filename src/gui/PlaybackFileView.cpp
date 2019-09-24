#include "PlaybackFileView.hpp"
#include "ui_playbackfileview.h"
#include <QFileDialog>

PlaybackFileView::PlaybackFileView(QWidget *parent)
    : QWidget(parent), ui(new Ui::PlaybackFileView) {
  ui->setupUi(this);
}

PlaybackFileView::~PlaybackFileView() { delete ui; }

QString PlaybackFileView::getFileName() const {
  return ui->lineEditFile->text();
}

void PlaybackFileView::selectFile() {

  auto fileName = QFileDialog::getOpenFileName(
      nullptr, "Open File", QDir::home().path(), "Serial (*.serial)");

  ui->lineEditFile->setText(fileName);
}

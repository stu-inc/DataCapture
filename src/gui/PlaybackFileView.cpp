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

void PlaybackFileView::setTimeText(const QString &text) {
  ui->labelTime->setText(text);
}

bool PlaybackFileView::getLoop() const { return ui->checkBoxLoop->isChecked(); }

void PlaybackFileView::setLoop(bool b) { ui->checkBoxLoop->setChecked(b); }

void PlaybackFileView::selectFile() {

  auto fileName = QFileDialog::getOpenFileName(
      nullptr, "Open File", QDir::home().path(), "Data (*.data)");

  ui->lineEditFile->setText(fileName);
}

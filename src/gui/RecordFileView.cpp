#include "RecordFileView.hpp"
#include "ui_recordfileview.h"
#include <QFileDialog>

RecordFileView::RecordFileView(QWidget *parent)
    : QWidget(parent), ui(new Ui::RecordFileView) {
  ui->setupUi(this);
}

RecordFileView::~RecordFileView() { delete ui; }

QString RecordFileView::getFileName() const { return ui->lineEditFile->text(); }

void RecordFileView::setTimeText(const QString &timeText) {
  ui->labelTime->setText(timeText);
}

void RecordFileView::setSizeText(const QString &sizeText) {
  ui->labelSize->setText(sizeText);
}

void RecordFileView::selectFile() {

  auto fileName = QFileDialog::getSaveFileName(
      nullptr, "Save File", QDir::home().path(), "Data (*.data)");

  ui->lineEditFile->setText(fileName);
}

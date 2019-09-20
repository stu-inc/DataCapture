#pragma once

#include <QWidget>

namespace Ui {
class RecordFileView;
}

class RecordFileView : public QWidget {
  Q_OBJECT
public:
  explicit RecordFileView(QWidget *parent = nullptr);
  ~RecordFileView();

  QString getFileName() const;

protected slots:
  void selectFile();

private:
  Ui::RecordFileView *ui;
};

#pragma once

#include <QWidget>

class SerialRecorder;

namespace Ui {
class RecordView;
}

class RecordView : public QWidget {
  Q_OBJECT
public:
  explicit RecordView(QWidget *parent = nullptr);
  ~RecordView();

protected slots:
  void startRecord();
  void stopRecord();

private:
  Ui::RecordView *ui;

  QSharedPointer<SerialRecorder> mRecorder;
};

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
  ~RecordView() override;

protected slots:
  void startRecord();
  void stopRecord();

protected:
  virtual void timerEvent(QTimerEvent *e) override;

private:
  Ui::RecordView *ui;

  QSharedPointer<SerialRecorder> mRecorder;
  int mTimerId = -1;
};

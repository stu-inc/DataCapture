#pragma once

#include <QWidget>

class SerialRecorder;
class UdpRecorder;
class TcpRecorder;

namespace Ui {
class RecordView;
}

class RecordView : public QWidget {
  Q_OBJECT
public:
  explicit RecordView(QWidget *parent = nullptr);
  virtual ~RecordView() override;

protected slots:
  void startRecord();
  void stopRecord();

  void changeInterface(int index);

protected:
  virtual void timerEvent(QTimerEvent *e) override;

private:
  Ui::RecordView *ui;

  QSharedPointer<SerialRecorder> mSerialRecorder;
  QSharedPointer<UdpRecorder> mUdpRecorder;
  QSharedPointer<TcpRecorder> mTcpRecorder;
  int mTimerId = -1;
};

#pragma once

#include <QWidget>

class SerialPlayer;
class UdpPlayer;
class TcpPlayer;

namespace Ui {
class PlaybackView;
}

class PlaybackView : public QWidget {
  Q_OBJECT
public:
  explicit PlaybackView(QWidget *parent = nullptr);
  ~PlaybackView() override;

protected slots:
  void startPlayback();
  void stopPlayback();

  void changeInterface(int index);

protected:
  virtual void timerEvent(QTimerEvent *e) override;

private:
  Ui::PlaybackView *ui;

  QSharedPointer<SerialPlayer> mSerialPlayer;
  QSharedPointer<UdpPlayer> mUdpPlayer;
  QSharedPointer<TcpPlayer> mTcpPlayer;
  int mTimerId = -1;
};

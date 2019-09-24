#pragma once

#include <QWidget>

class SerialPlayer;

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

protected:
  virtual void timerEvent(QTimerEvent *e) override;

private:
  Ui::PlaybackView *ui;

  QSharedPointer<SerialPlayer> mPlayer;
  int mTimerId = -1;
};

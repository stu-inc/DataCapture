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
  ~PlaybackView();

protected slots:
  void startPlayback();
  void stopPlayback();

private:
  Ui::PlaybackView *ui;

  QSharedPointer<SerialPlayer> mPlayer;
};

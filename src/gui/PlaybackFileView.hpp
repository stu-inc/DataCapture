#pragma once

#include <QWidget>

namespace Ui {
class PlaybackFileView;
}

class PlaybackFileView : public QWidget {
  Q_OBJECT
public:
  explicit PlaybackFileView(QWidget *parent = nullptr);
  ~PlaybackFileView();

  QString getFileName() const;
  void setTimeText(const QString &text);

  bool getLoop() const;
  void setLoop(bool b);

protected slots:
  void selectFile();

private:
  Ui::PlaybackFileView *ui;
};

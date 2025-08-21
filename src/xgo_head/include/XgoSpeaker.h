#ifndef XGOSPEAKER_H
#define XGOSPEAKER_H


#include <QObject>
#include <QMediaPlayer>
#include "TLog.h"


class XgoSpeaker : public QObject
{
  Q_OBJECT
public:
  XgoSpeaker();
  virtual ~XgoSpeaker();

  bool isBusy();
  void playAudio(QUrl url);
  void stop();

public slots:
  void onStatChanged2(QMediaPlayer::State state);

private:
  QMediaPlayer mPlayer;
  bool mBusy = false;




};

#endif // XGOSPEAKER_H

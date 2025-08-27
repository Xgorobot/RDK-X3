#include "XgoSpeaker.h"

XgoSpeaker::XgoSpeaker()
{
  //  using QMediaPlayer::State;

  // connect(&this->mPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onStatChanged2(QMediaPlayer::State)));
}

void XgoSpeaker::playAudio(QUrl url)
{
  TLog::d(QString("play audio %1").arg(url.toString()));
  this->mBusy = true;

  this->mPlayer.setMedia(url);
  this->mPlayer.setVolume(100);
  this->mPlayer.play();
}


XgoSpeaker::~XgoSpeaker()
{

}

bool XgoSpeaker::isBusy()
{
  return this->mBusy;
}

void XgoSpeaker::stop()
{
  this->mPlayer.stop();

  this->mBusy = false;
}


void XgoSpeaker::onStatChanged2(QMediaPlayer::State state)
{
  if(state == QMediaPlayer::PlayingState)
  {
    this->mBusy = true;
  }
  else
  {
    this->mBusy = false;
  }
}

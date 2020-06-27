#ifndef FVAPLAYER_H
#define FVAPLAYER_H

#include "videowidget.h"

#include <QtWidgets\QWidget>
#include <QtMultimedia\QMediaPlayer>

QT_BEGIN_NAMESPACE
class QLabel;
class QMediaPlayer;
class QPushButton;
class QSlider;
class QVideoWidget;
QT_END_NAMESPACE

class FVAPlayer : public QWidget
{
	Q_OBJECT

	public:
		FVAPlayer(QWidget *parent = 0);
		~FVAPlayer();

	signals:
		void fullScreenChanged(bool fullScreen);

	private slots:

		void durationChanged(qint64 duration);
		void positionChanged(qint64 progress);
		void metaDataChanged();
		void seek(int seconds);
		void statusChanged(QMediaPlayer::MediaStatus status);
		void bufferingProgress(int progress);
		void videoAvailableChanged(bool available);
		void displayErrorMessage();

	private:
		void setTrackInfo(const QString &info);
		void setStatusInfo(const QString &info);
		void handleCursor(QMediaPlayer::MediaStatus status);
		void updateDurationInfo(qint64 currentInfo);

		QMediaPlayer *player;
		VideoWidget *videoWidget;
		QLabel *coverLabel;
		QSlider *slider;
		QLabel *labelDuration;
		QPushButton *fullScreenButton;
		QString trackInfo;
		QString statusInfo;
		qint64 duration;
};

#endif // PLAYER_H

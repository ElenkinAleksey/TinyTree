#ifndef PLAYER_H
#define PLAYER_H

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaContent>
#include <QDebug>
#include <QObject>
#include <QTime>
#include <QMetaDataReaderControl>
#include <QMediaMetaData>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <QFileInfo>
#include "filecontroller.h"

#include "taglib.h"
#include <tbytevector.h>//ByteVector
#include <mpegfile.h>//mp3 file
#include <id3v2tag.h>//tag
#include <id3v2frame.h>//frame
#include <attachedpictureframe.h>
#include <iostream>
#include <stdio.h>
#include <fstream>

class Player : public QObject
{
    Q_OBJECT
    QMediaPlayer *mediaPlayer;
    qint64 trackDuration;
    int trackCount;
    QStringList metaData;
public:
    Player();
    ~Player();
    QMediaPlaylist *mediaPlaylist;
    QString artistName, albumName, trackName, trackYear, trackGenre, bitRate;
    QMediaPlayer *getMediaPlayer() const;
    QVariant trackUrl;

    int getTemplate();
    qint64 getTrackDuration() const;
    bool repeatTrackFlag;

signals:
    void trackPositionChanged(qint64 &progress);
    void infoUpdated(QString&, QString&, QString&);
    void setTrackInfoToView(QString&, QString&, QString&, QString&, QString&, QString&);
    void checkTemplate(int);
    void playerStopped();
    void addTrackToPlaylist(int number, QString title, QString artist, QString album);

public slots:
    void playTrack();
    void pauseTrack();
    void changeVolume(int);
    void positionChanged(qint64);
    void positionChangedFromUser(qint64);
    void updateDurationInfo(qint64);
    void setTrackInfoToView();
    void addToPlaylist(QString);
    void goToNextTrack();
    void goToPreviousTrack();
    void dataChanged();
    void changeTags(QString artist, QString title, QString album);

//    void open();
};

#endif // PLAYER_H

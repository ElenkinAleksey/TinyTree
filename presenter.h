#ifndef PRESENTER_H
#define PRESENTER_H

#include <QObject>
#include <QQmlProperty>
#include "player.h"
//#include "filecontroller.h"


class Presenter : public QObject
{

    Q_OBJECT
public:
    Presenter(QObject *QMLObject);
    ~Presenter();
    Player *player;
    QVariant getTrackURL();
    bool playFlag;

    Q_INVOKABLE QString getSongTextFromFile();
protected:
    QObject *viewer, *qmlPlayer;
    QObject *volumeSlider;
    QObject *muteButton, *playButton, *repeatButton;
    QObject* trackSlider;
    QObject* currentPositionLabel;
    QObject* trackDurationLabel;
    QObject* estimatedDurationLabel, *trackTemplate;
    QObject *artistName, *albumName, *trackName;
    QObject *artistNameInfo, *titleInfo, *albumInfo, *yearInfo, *genreInfo, *bitRateInfo;
signals:
    void playTrack();
    void pauseTrack();

public slots:
    void playButtonClicked();
    void volumeChanged();
    void mute();
    void trackPositionChanged(qint64&);
    void positionChangedFromUser();
    void infoChanged(QString&, QString&, QString&);
    void setTrackInfoToView(QString&, QString&, QString&, QString&, QString&, QString&);
    void addToPlaylist(QVariant trackPath);
    void goToNextTrack();
    void goToPreviousTrack();
    void repeatTrackButtonPressed();
    void shuffleButtonPressed();
    void checkTemplate(int);
    void playerStopped();
};

#endif // PRESENTER_H

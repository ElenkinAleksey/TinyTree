#include "presenter.h"
#include "iplayer.h"
#include <QThread>

Presenter::Presenter(QObject *QMLObject) : viewer (QMLObject)
{
    this->player = new Player;
    playFlag = false;
    volumeSlider = viewer->findChild<QObject*>("volumeSlider");
    muteButton = viewer->findChild<QObject*>("muteButton");
    trackSlider = viewer->findChild<QObject*>("trackSlider");
    currentPositionLabel = viewer->findChild<QObject*>("currentPositionLabel");
    trackDurationLabel = viewer->findChild<QObject*>("trackDurationLabel");
    estimatedDurationLabel = viewer->findChild<QObject*>("estimatedDurationLabel");
    artistName = viewer->findChild<QObject*>("artistName");
    albumName = viewer->findChild<QObject*>("albumName");
    trackName = viewer->findChild<QObject*>("trackName");
    playButton = viewer->findChild<QObject*>("playButton");
    repeatButton = viewer->findChild<QObject*>("repeatButton");
    qmlPlayer = viewer->findChild<QObject*>("playMusic");
    trackTemplate = viewer->findChild<QObject*>("trackTemplate");
    artistNameInfo = viewer->findChild<QObject*>("artistNameInfo");
    titleInfo = viewer->findChild<QObject*>("trackNameInfo");
    albumInfo = viewer->findChild<QObject*>("albumNameInfo");
    yearInfo = viewer->findChild<QObject*>("yearInfo");
    genreInfo = viewer->findChild<QObject*>("genreInfo");
    bitRateInfo = viewer->findChild<QObject*>("bitRateInfo");
    QQmlProperty::write(currentPositionLabel, "text", "");
    QQmlProperty::write(trackDurationLabel, "text", "");
    QQmlProperty::write(estimatedDurationLabel, "text", "");
    connect(player, SIGNAL(trackPositionChanged(qint64&)), SLOT(trackPositionChanged(qint64&)));
    connect(player,SIGNAL(infoUpdated(QString&,QString&,QString&)), SLOT(infoChanged(QString&,QString&,QString&)));
    connect(player,SIGNAL(setTrackInfoToView(QString&,QString&,QString&,QString&,QString&, QString&)), SLOT(setTrackInfoToView(QString&,QString&,QString&,QString&,QString&, QString&)));
    connect(player,SIGNAL(checkTemplate(int)),SLOT(checkTemplate(int)));
    connect(player,SIGNAL(playerStopped()),SLOT(playerStopped()));
}

Presenter::~Presenter()
{
    delete this->player;
}

QVariant Presenter::getTrackURL()
{
    return player->trackUrl;
}

QString Presenter::getSongTextFromFile()
{
    QString artist = QVariant(player->getMediaPlayer()->metaData(QMediaMetaData::ContributingArtist)).toString();
    QString title = QVariant(player->getMediaPlayer()->metaData(QMediaMetaData::Title)).toString();
    QString trackFilePath = "/home/alexey/Dropbox/Workspace/TinyTreeQMLVersion/TrackLib/" + artist + " - " + title+ ".track";
    QString songText = FileController::getSongTextFromFile(trackFilePath);
    qDebug() << "TEXT: " << songText;
    if (songText == "\n") {
        return "There's no text available";
    } else {
        return songText;
    }
}

void Presenter::editTags(QVariant artist, QVariant title, QVariant album)
{
    QString artistString = artist.toString();
    QString titleString = title.toString();
    QString albumString = album.toString();
    player->changeTags(artistString, titleString, albumString);
}

void Presenter::playButtonClicked()
{   if (player->mediaPlaylist->isEmpty() == false){
        if (playFlag == false) {
            player->playTrack();
            playFlag = true;
            QQmlProperty::write(playButton, "source", "qrc:/pauseIcon.svg");
            QQmlProperty::write(trackSlider, "maximumValue", player->getMediaPlayer()->duration()/1000);
        } else {
            player->pauseTrack();
            playFlag = false;
            QQmlProperty::write(playButton, "source", "qrc:/playIcon.svg");
        }
    }
}

void Presenter::volumeChanged()
{
    player->changeVolume(this->volumeSlider->property("value").toInt());
}

void Presenter::mute()
{
    if (this->muteButton->property("source") == "qrc:/speakerIcon.svg") {
        player->getMediaPlayer()->setMuted(true);
    }
    if (this->muteButton->property("source") == "qrc:/mutedIcon.svg") {
        player->getMediaPlayer()->setMuted(false);
    }
}

void Presenter::trackPositionChanged(qint64 &progress)
{
    QQmlProperty::write(trackSlider, "value", progress/1000);
    QQmlProperty::write(qmlPlayer, "source", player->trackUrl.toString());
    QQmlProperty::write(trackSlider, "maximumValue", player->getMediaPlayer()->duration()/1000);
//    qDebug() << "setSliderValue: " << progress;
}

void Presenter::positionChangedFromUser()
{
    player->positionChangedFromUser(trackSlider->property("value").toUInt());
    QQmlProperty::write(trackSlider, "maximumValue", player->getMediaPlayer()->duration()/1000);
}

void Presenter::infoChanged(QString &currentPosition, QString &trackDuration, QString &estimatedDuration)
{
    QQmlProperty::write(currentPositionLabel, "text", currentPosition);
    QQmlProperty::write(trackDurationLabel, "text", trackDuration);
    QQmlProperty::write(estimatedDurationLabel, "text", estimatedDuration);
}

void Presenter::setTrackInfoToView(QString &artist, QString &album, QString &title, QString &trackYear, QString &trackGenre, QString &bitrate)
{
    QQmlProperty::write(artistName, "text", QVariant(artist));
    QQmlProperty::write(trackName, "text", QVariant(title));
    QQmlProperty::write(albumName, "text", QVariant(album));
    QQmlProperty::write(artistNameInfo, "text", QVariant(artist));
    QQmlProperty::write(albumInfo, "text", QVariant(album));
    QQmlProperty::write(titleInfo, "text", QVariant(title));
    QQmlProperty::write(yearInfo, "text", QVariant(trackYear));
    QQmlProperty::write(genreInfo, "text", QVariant(trackGenre));
//    QQmlProperty::write(bitRateInfo, "text", QVariant(bitrate));
}

void Presenter::addToPlaylist(QVariant trackPath)
{
    QString path = trackPath.toString();
    if (path != "") {
        player->addToPlaylist(path);
    }
    qDebug() << "wanttoaddtrack";
}

void Presenter::goToNextTrack()
{
    player->goToNextTrack();
}

void Presenter::goToPreviousTrack()
{
    player->goToPreviousTrack();
}

void Presenter::repeatTrackButtonPressed()
{
    qDebug() << "repeatTrackPressed";
    if (player->mediaPlaylist->isEmpty() == false){
        if (player->repeatTrackFlag == false){
            player->repeatTrackFlag = true;
            QQmlProperty::write(repeatButton, "source", "qrc:/reload_pressed.svg");
        } else {
            player->repeatTrackFlag = false;
            QQmlProperty::write(repeatButton, "source", "qrc:/reload.svg");
        }
    }
}

void Presenter::shuffleButtonPressed()
{
    if (player->mediaPlaylist->isEmpty() == false){
        player->mediaPlaylist->shuffle();
    }
}

void Presenter::checkTemplate(int answer)
{
    if (answer == 0) {
        qDebug () << "template doesn't exists";
        QString artist = QVariant(player->getMediaPlayer()->metaData(QMediaMetaData::ContributingArtist)).toString();
        QString title = QVariant(player->getMediaPlayer()->metaData(QMediaMetaData::Title)).toString();
        bool fileExists = FileController::checkTrackFile(artist, title);
        if (fileExists == false) {
            QQmlProperty::write(trackTemplate, "source", "");
            QQmlProperty::write(trackTemplate, "source", "qrc:/template_not_found.jpg");
        } else{
            QString url = FileController::getCoverURLFromFile("/home/alexey/Dropbox/Workspace/TinyTreeQMLVersion/TrackLib/" + artist + " - " + title + ".track");
            QString templatePath = "/home/alexey/Dropbox/Workspace/TinyTreeQMLVersion/Templates/" + artist + " - " + title + ".jpg";
            QFile templateFile(templatePath);
            bool templateFileExists = templateFile.exists();
            if (templateFileExists == true){
                QQmlProperty::write(trackTemplate, "source", "");
                QQmlProperty::write(trackTemplate, "source", "file://" + templatePath);
            } else {
                if (url == "") {
                    QQmlProperty::write(trackTemplate, "source", "");
                    QQmlProperty::write(trackTemplate, "source", "qrc:/template_not_found.jpg");
                } else {
                    QString templateFilePath = WebController::getCoverArtFromURL(url, artist, title);
                    qDebug() << "GET TEMPLATE FROM URL";
                    if (templateFilePath == "") {
                        QQmlProperty::write(trackTemplate, "source", "");
                        QQmlProperty::write(trackTemplate, "source", "qrc:/template_not_found.jpg");
                    } else {
                        QQmlProperty::write(trackTemplate, "source", "");
                        QQmlProperty::write(trackTemplate, "source", "file://" + templateFilePath);
                    }
                }
            }
        }
    } else {
        qDebug () << "template exists";
        QQmlProperty::write(trackTemplate, "source", "");
        QQmlProperty::write(trackTemplate, "source", "file:///home/alexey/test.jpg");
    }
}

void Presenter::playerStopped()
{
    QQmlProperty::write(playButton, "source", "qrc:/playIcon.svg");
}

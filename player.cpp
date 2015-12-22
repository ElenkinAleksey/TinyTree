#include "player.h"

using namespace std;
using namespace TagLib::ID3v2;

QMediaPlayer *Player::getMediaPlayer() const
{
    return mediaPlayer;
}



qint64 Player::getTrackDuration() const
{
    return trackDuration;
}

Player::Player()
{
    mediaPlayer = new QMediaPlayer;
    mediaPlaylist = new QMediaPlaylist;
    mediaPlayer->setPlaylist(mediaPlaylist);
    mediaPlayer->setVolume(50);
    this->repeatTrackFlag = false;
    QObject::connect(mediaPlayer,SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(mediaPlaylist,SIGNAL(currentIndexChanged(int)),SLOT(dataChanged()));
}

Player::~Player()
{
    delete mediaPlayer;
}

void Player::playTrack()
{
    if (mediaPlayer->isMetaDataAvailable() == true){
        mediaPlayer->play();
        trackUrl = mediaPlayer->currentMedia().canonicalUrl().toString();
        QString artist = QVariant(mediaPlayer->metaData(QMediaMetaData::ContributingArtist)).toString();
        QString track = QVariant(mediaPlayer->metaData(QMediaMetaData::Title)).toString();
        QString trackFilePath = "/home/alexey/Dropbox/Workspace/TinyTreeQMLVersion/TrackLib/" + artist + " - " + track + ".track";
        QString album = QVariant(mediaPlayer->metaData(QMediaMetaData::AlbumTitle)).toString();
        QString year = QVariant(mediaPlayer->metaData(QMediaMetaData::Year)).toString();
        QString genre = QVariant(mediaPlayer->metaData(QMediaMetaData::Genre)).toString();

        if (FileController::checkTrackFile(artist, track) == false) {
            FileController::createTrackFile(trackUrl.toString(), trackFilePath, artist, track, album, year, genre);
        } else {
            //            FileController::editTrackFile(trackFilePath, artist, track, album, year, genre);
        }
        //    qDebug() << mediaPlayer->duration();
        qDebug() << mediaPlayer->currentMedia().canonicalUrl().toString();

        setTrackInfoToView();
        getTemplate();
    } else {
        mediaPlayer->pause();
    }
    //    updateDurationInfo(mediaPlayer->position());
}

void Player::pauseTrack()
{
    mediaPlayer->pause();
}

void Player::changeVolume(int volume)
{
    mediaPlayer->setVolume(volume);
}

void Player::positionChanged(qint64 progress)
{
    trackUrl = mediaPlayer->currentMedia().canonicalUrl().toString();
    if (progress != 0){
        if ((progress > mediaPlayer->duration() - 1000) && (repeatTrackFlag == true)){
            mediaPlayer->setPosition(1);
        } else { if ((mediaPlaylist->currentIndex() == mediaPlaylist->mediaCount() - 1) && (mediaPlayer->position() > mediaPlayer->duration() - 100)) {
                mediaPlayer->stop();
                emit playerStopped();
            } else {
                if (mediaPlayer->position() < 1000) {
                    QString artist = QVariant(mediaPlayer->metaData(QMediaMetaData::ContributingArtist)).toString();
                    QString track = QVariant(mediaPlayer->metaData(QMediaMetaData::Title)).toString();
                    QString trackFilePath = "/home/alexey/Dropbox/Workspace/TinyTreeQMLVersion/TrackLib/" + artist + " - " + track + ".track";
                    QString album = QVariant(mediaPlayer->metaData(QMediaMetaData::AlbumTitle)).toString();
                    QString year = QVariant(mediaPlayer->metaData(QMediaMetaData::Year)).toString();
                    QString genre = QVariant(mediaPlayer->metaData(QMediaMetaData::Genre)).toString();

                    if (FileController::checkTrackFile(artist, track) == false) {
                        FileController::createTrackFile(trackUrl.toString(), trackFilePath, artist, track, album, year, genre);
                    } else {
                        //            FileController::editTrackFile(trackFilePath, artist, track, album, year, genre);
                    }
                    getTemplate();
                }

                //    qDebug() << "Track URL: " << trackUrl.toString();
                emit trackPositionChanged(progress);
                updateDurationInfo(mediaPlayer->position());
                setTrackInfoToView();
            }
        }
    }
}

void Player::positionChangedFromUser(qint64 newposition)
{
    mediaPlayer->pause();
    mediaPlayer->setPosition(newposition*1000);
    mediaPlayer->play();
    if ((newposition > mediaPlayer->duration() - 1000) && (repeatTrackFlag == true)){
        mediaPlayer->setPosition(1);
    }
    trackUrl = mediaPlayer->currentMedia().canonicalUrl().toString();
    updateDurationInfo(mediaPlayer->position());
}

void Player::updateDurationInfo(qint64 currentInfo)
{
    QString currentPosition;
    QString estimatedDuration;
    QString trackDurationstr;
    this->trackDuration = mediaPlayer->duration();
    if (currentInfo || trackDuration) {
        QTime currentTime((currentInfo/1000/3600)%60, (currentInfo/1000/60)%60, currentInfo/1000%60, (currentInfo/1000*1000)%1000);
        QTime totalTime((trackDuration/1000/3600)%60, (trackDuration/1000/60)%60, trackDuration/1000%60, (trackDuration/1000*1000)%1000);
        QTime estimatedTime((trackDuration/1000 - currentInfo/1000)/3600%60, (trackDuration - currentInfo)/1000/60%60, (trackDuration - currentInfo)/1000%60, (trackDuration - currentInfo)/1000*1000%1000);
        QString format = "mm:ss";
        if (trackDuration > 3600*60*1000)
            format = "hh:mm:ss";
        trackDurationstr = totalTime.toString(format);
        currentPosition = currentTime.toString(format);
        estimatedDuration = estimatedTime.toString(format);
        emit infoUpdated(currentPosition, trackDurationstr, estimatedDuration);
    }

}

void Player::setTrackInfoToView()
{
    if (mediaPlayer->isMetaDataAvailable()){
        artistName = QVariant(mediaPlayer->metaData(QMediaMetaData::ContributingArtist)).toString();
        albumName = QVariant(mediaPlayer->metaData(QMediaMetaData::AlbumTitle)).toString();
        trackName = QVariant(mediaPlayer->metaData(QMediaMetaData::Title)).toString();
        trackYear = QVariant(mediaPlayer->metaData(QMediaMetaData::Year)).toString();
        trackGenre = QVariant(mediaPlayer->metaData(QMediaMetaData::Genre)).toString();
        bitRate = QVariant(mediaPlayer->metaData(QMediaMetaData::AudioBitRate)).toString();
        //        getTemplate();
        emit setTrackInfoToView(artistName, albumName, trackName, trackYear, trackGenre, bitRate);
        //            qDebug() << mediaPlayer->metaData(QMediaMetaData::CoverArtImage);

    }
}

void Player::addToPlaylist(QString filePath)
{
    QUrl url = QUrl::fromLocalFile(filePath);
    qDebug() << filePath;
    if (mediaPlaylist->isEmpty() == true){
        mediaPlaylist->addMedia(url);
        mediaPlaylist->setCurrentIndex(0);
        qDebug() << mediaPlaylist->mediaCount();
        qDebug() << "Current index: " << mediaPlaylist->currentIndex();

    } else {
        mediaPlaylist->addMedia(url);
        qDebug() << mediaPlaylist->mediaCount();
        qDebug() << "Current index: " << mediaPlaylist->currentIndex();
    }
}

void Player::goToNextTrack()
{
    if (mediaPlaylist->currentIndex() == mediaPlaylist->mediaCount() - 1) { mediaPlaylist->setCurrentIndex(0);  setTrackInfoToView(); getTemplate();} else {
        mediaPlaylist->next();
        setTrackInfoToView();
        getTemplate();
        qDebug() << "Current index: " << mediaPlaylist->currentIndex();
        qDebug() << "gotonexttrack";
    }
}

void Player::goToPreviousTrack()
{
    if (mediaPlaylist->currentIndex() == 0) { mediaPlaylist->setCurrentIndex(mediaPlaylist->mediaCount() - 1);  setTrackInfoToView(); getTemplate();} else {
        mediaPlaylist->previous();
        qDebug() << "Current index: " << mediaPlaylist->currentIndex();
    }
    setTrackInfoToView();
    getTemplate();
}

void Player::dataChanged()
{
    setTrackInfoToView();
    getTemplate();
}

int Player::getTemplate()
{
    TagLib::MPEG::File mp3File(mediaPlayer->currentMedia().canonicalUrl().toString().replace("file://","").toLocal8Bit().data());
    qDebug() << "Gettingfrom: " << mediaPlayer->currentMedia().canonicalUrl().toString().replace("file://","");
    Tag * mp3Tag;
    FrameList listOfMp3Frames;
    AttachedPictureFrame * PicFrame;
    void *RetImage = NULL, *SrcImage ;
    unsigned long Size ;

    FILE *jpegFile;
    jpegFile = fopen("/home/alexey/test.jpg","wb");

    mp3Tag= mp3File.ID3v2Tag();
    if(mp3Tag)
    {
        listOfMp3Frames = mp3Tag->frameListMap()["APIC"];//look for picture frames only
        if(!listOfMp3Frames.isEmpty())
        {
            FrameList::ConstIterator it= listOfMp3Frames.begin();
            for(it; it != listOfMp3Frames.end() ; ++it)
            {
                PicFrame = (TagLib::ID3v2::AttachedPictureFrame *)(*it);//cast Frame * to AttachedPictureFrame*
                if ( PicFrame->type() ==
                     TagLib::ID3v2::AttachedPictureFrame::FrontCover)
                {
                    Size = PicFrame->picture().size() ;
                    SrcImage = malloc ( Size ) ;
                    if ( SrcImage )
                    {
                        memcpy ( SrcImage, PicFrame->picture().data(), Size ) ;
                        fwrite(SrcImage,Size,1,jpegFile);
                        fclose(jpegFile);
                        free( SrcImage ) ;
                    }
                    qDebug() <<" The picture has been written to outputFile.jpg  \n";
                    emit checkTemplate(1);
                    return 1;
                }
            }
        }
    }
    emit checkTemplate(0);
    return 0;
}






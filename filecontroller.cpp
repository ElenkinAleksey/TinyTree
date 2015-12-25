#include "filecontroller.h"

FileController::FileController()
{

}

bool FileController::checkTrackFile(QString artistName, QString trackName)
{
    QString trackFilePath = "/home/alexey/Dropbox/Workspace/TinyTreeQMLVersion/TrackLib/" + artistName + " - " + trackName + ".track";
    qDebug() << trackFilePath;
    qDebug() << QFileInfo::exists(trackFilePath);
    if (QFileInfo::exists(trackFilePath) == true) {
        return true;
    } else {
        return false;
    }
}

void FileController::createTrackFile(QString trackPath, QString trackFilePath, QString artistName, QString trackName, QString albumName, QString year, QString genre)
{
    QFile trackFile(trackFilePath);
    trackFile.open(QIODevice::ReadWrite);
    editTrackFile(trackPath, trackFilePath ,artistName, trackName, albumName, year, genre);
    trackFile.close();
}

void FileController::editTrackFile(QString trackPath, QString trackFilePath, QString artistName, QString trackName, QString albumName, QString year, QString genre)
{
    QFile trackFile(trackFilePath);
    QStringList lyricsAndCoverURL;
    QString lyrics;
    QString coverArtURL;
    lyricsAndCoverURL = WebController::getLyricsAndCoverURL(artistName, trackName);
    lyrics = lyricsAndCoverURL[1];
    coverArtURL = lyricsAndCoverURL[0];
    trackFile.resize(0);
    if (trackFile.isOpen() == true) {
        trackFile.write((trackPath.replace("file://","") + "\n"
                         + artistName + "\n"
                         + trackName + "\n"
                         + albumName + "\n"
                         + year + "\n"
                         + genre + "\n"
                         + coverArtURL + "\n"
                         + lyrics).toUtf8().data());
    } else {
        trackFile.open(QIODevice::ReadWrite);
        trackFile.write((trackPath.replace("file://","") + "\n"
                         + artistName + "\n"
                         + trackName + "\n"
                         + albumName + "\n"
                         + year + "\n"
                         + genre + "\n"
                         + coverArtURL + "\n"
                         + lyrics).toUtf8().data());
        trackFile.close();
    }
}


QString FileController::getCoverURLFromFile(QString trackFilePath)
{
    QFile file(trackFilePath);
    QString url;
    file.open(QIODevice::ReadOnly);
    while (!file.atEnd()) {
       QString thisLine = file.readLine();
       if (thisLine == "Cover art URL:\n") {
           url = file.readLine().replace("\n","");
           qDebug() << url;
       }
    }
    file.close();
    return url;
}

QString FileController::getSongTextFromFile(QString trackFilePath)
{
    QFile file(trackFilePath);
    QString text;
    file.open(QIODevice::ReadOnly);
    while (!file.atEnd()) {
       QString thisLine = file.readLine();
       if (thisLine == "Lyrics:\n") {
           while (!file.atEnd()) {
           text += file.readLine();
           }
       }
    }
    file.close();
    return text;
}

QStringList FileController::getMetaDataFromFile(QString url)
{
    TagLib::MPEG::File mp3File(url.replace("file://","").toLocal8Bit().data());
    QStringList metaData;
    metaData.push_back(QString(mp3File.ID3v2Tag()->title().toCString()));
    metaData.push_back(QString(mp3File.ID3v2Tag()->artist().toCString()));
    metaData.push_back(QString(mp3File.ID3v2Tag()->album().toCString()));
    return metaData;
}

void FileController::editTags(QString url, QString artist, QString title, QString album)
{
    TagLib::MPEG::File mp3File(url.replace("file://","").toLocal8Bit().data());
    mp3File.ID3v2Tag()->setArtist(artist.toStdString());
    mp3File.ID3v2Tag()->setTitle(title.toStdString());
    mp3File.ID3v2Tag()->setAlbum(album.toStdString());
    mp3File.save();
}



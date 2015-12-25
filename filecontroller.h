#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QStringList>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QEventLoop>
#include <QStringList>
#include <QXmlStreamReader>
#include "webcontroller.h"

#include "taglib.h"
#include <tbytevector.h>//ByteVector
#include <mpegfile.h>//mp3 file
#include <id3v2tag.h>//tag
#include <id3v2frame.h>//frame

class FileController
{

public:
    explicit FileController();
    static bool checkTrackFile(QString artistName, QString trackName);
    static void createTrackFile(QString trackPath, QString trackFilePath,QString artistName, QString trackName, QString albumName, QString year, QString genre);
    static void editTrackFile(QString trackPath, QString trackFilePath, QString artistName, QString trackName, QString albumName, QString year, QString genre);
    static QString getCoverURLFromFile(QString trackFilePath);
    static QString getSongTextFromFile(QString trackFilePath);
    static QStringList getMetaDataFromFile(QString url);
    static void editTags(QString url, QString artist, QString title, QString album);

signals:

public slots:
//    void parseLyric();

};

#endif // FILECONTROLLER_H

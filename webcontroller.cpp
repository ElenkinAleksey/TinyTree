#include "webcontroller.h"

WebController::WebController(QObject *parent) : QObject(parent)
{

}

QStringList WebController::getLyricsAndCoverURL(QString artist, QString title)
{
    QEventLoop eventLoop;
    QNetworkAccessManager manager;
    QString artistLowercase = artist.toLower();
    QString titleLowercase = title.toLower();
    QStringList lyricsAndCoverURL;
    QString lyrics, coverArtURL;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop,SLOT(quit()));

    QNetworkRequest request(QUrl(QString("http://api.chartlyrics.com/apiv1.asmx/SearchLyricDirect?artist=") + artistLowercase + QString("&song=") + titleLowercase));
    QNetworkReply *reply = manager.get(request);
    eventLoop.exec();
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Success";

        QXmlStreamReader xml;
        xml.addData(reply->readAll().data());
        lyricsAndCoverURL.push_back("");
        lyricsAndCoverURL.push_back("");
        while (!xml.atEnd() && !xml.hasError()) {
//            qDebug() << "wthErr";
            QXmlStreamReader::TokenType token = xml.readNext();
            if (token == QXmlStreamReader::StartDocument){
                continue; }
            if (token == QXmlStreamReader::StartElement) {
//                qDebug() << "strtEl";
                if (xml.name() == "Lyric"){
//                    qDebug() << "found";
                    lyrics = "Lyrics:\n" + xml.readElementText();
                    lyricsAndCoverURL[0] = lyrics;
//                    qDebug() << "Lyrics: " << lyrics;
                }
                if (xml.name() == "LyricCovertArtUrl"){
//                    qDebug() << "found";
                    coverArtURL = "Cover art URL:\n" + xml.readElementText() + "\n";
                    lyricsAndCoverURL[1] = coverArtURL;
//                    qDebug() << "coverArtURL: " << coverArtURL;
                }
            }
        }
//        qDebug() << xml.errorString().toUtf8().data();
    } else {
        qDebug() << "Failure: " << reply->errorString();
        delete reply;
        lyricsAndCoverURL.push_back("");
        lyricsAndCoverURL.push_back("");
    }
    qDebug() << "FIRSTITEM: " << lyricsAndCoverURL[0];
    qDebug() << "SECONDITEM: " << lyricsAndCoverURL[1];
    return lyricsAndCoverURL;
}

QString WebController::getCoverArtFromURL(QString url, QString artist, QString title)
{
    QNetworkAccessManager manager;
    QEventLoop eventLoop;
    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &eventLoop,SLOT(quit()));
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply = manager.get(request);
    eventLoop.exec();

    QImage image;
    QImageReader reader(reply);
    QString templateFilePath;
     if (reply->error() == QNetworkReply::NoError) {
         reader.read(&image);
         image.save("/home/alexey/Dropbox/Workspace/TinyTreeQMLVersion/Templates/" + artist + " - " + title + ".jpg");
         templateFilePath = "/home/alexey/Dropbox/Workspace/TinyTreeQMLVersion/Templates/" + artist + " - " + title + ".jpg";
     } else {
         qDebug() << "Failure: " << reply->errorString();
     }
     return templateFilePath;
}

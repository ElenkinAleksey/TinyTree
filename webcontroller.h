#ifndef WEBCONTROLLER_H
#define WEBCONTROLLER_H

#include <QObject>
#include <QImage>
#include <QImageReader>
#include "filecontroller.h"

class WebController : public QObject
{
    Q_OBJECT
public:
    explicit WebController(QObject *parent = 0);
    static QStringList getLyricsAndCoverURL(QString artist, QString title);
    static QString getCoverArtFromURL(QString url, QString artist, QString title);
signals:

public slots:
};

#endif // WEBCONTROLLER_H

#ifndef PLAYLISTTABLEMODEL_H
#define PLAYLISTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVariant>
#include <QList>
#include "presenter.h"

struct dataStruct {
    int rowNumber;
    QString title;
    QString artist;
    QString album;

    dataStruct() {
        rowNumber = 0;
        title = "";
        artist = "";
        album = "";
    }

    dataStruct(int rowNumber, const QString title, const QString artist, const QString album) :
        rowNumber(rowNumber), title(title), artist(artist), album(album) {}


};

class PlaylistTableModel : public QAbstractTableModel
{

    Q_OBJECT
public:
    PlaylistTableModel(Presenter *presenter);
    dataStruct *mainStruct;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QHash<int, QByteArray>roleNames() const;

    QList<dataStruct*>tableDataList;

    enum myRoles {
        number = 1,
        title,
        artist,
        album
    };

signals:
public slots:
    void addTrackToPlaylist(int number, QString title, QString artist, QString album);
};

#endif // PLAYLISTTABLEMODEL_H

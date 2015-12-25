#include "playlisttablemodel.h"

PlaylistTableModel::PlaylistTableModel(Presenter *presenter)
{
    connect(presenter->player, SIGNAL(addTrackToPlaylist(int,QString,QString,QString)),this, SLOT(addTrackToPlaylist(int,QString,QString,QString)));
}

int PlaylistTableModel::rowCount(const QModelIndex &parent) const
{
    return tableDataList.count();
}

int PlaylistTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant PlaylistTableModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case number:
        return tableDataList.at(index.row())->rowNumber;
    case title:
        return tableDataList.at(index.row())->title;
    case artist:
        return tableDataList.at(index.row())->artist;
    case album:
        return tableDataList.at(index.row())->album;
    }

    return QVariant();
}

QHash<int, QByteArray> PlaylistTableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[number] = "role_number";
    roles[title] = "role_title";
    roles[artist] = "role_artist";
    roles[album] = "role_album";
    return roles;
}

void PlaylistTableModel::addTrackToPlaylist(int number, QString title, QString artist, QString album)
{

    mainStruct = new dataStruct(number, title, artist, album);
    tableDataList.append(mainStruct);
    qDebug() << title;
    QModelIndex topLeft = createIndex(0,0);
    QModelIndex bottomRight = createIndex(tableDataList.count()-1,4);
    emit layoutChanged();
    emit dataChanged(topLeft, bottomRight);
}




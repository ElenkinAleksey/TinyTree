import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtMultimedia 5.5
import QtTest 1.1
import QtQuick.Extras 1.4
import Qt.labs.folderlistmodel 2.1
import QtQuick.Dialogs 1.2
import QtMultimedia 5.0

Window {
    id: window
    visible: true
    width: 700
    height: 420
    MainForm {
        id: mainForm
        width: window.width
        height: window.height
        anchors.bottomMargin: -25
        anchors.fill: parent

        Audio {
                id: playMusic
                objectName: "playMusic"
            }

        Rectangle {
            id: mainRectangle
            property alias mouseArea: mouseArea
            property alias playButtonMouseArea : playButtonMouseArea
            property alias playButton: playButton
            property var selectedFiles: ""


            width: mainForm.width
            height: mainForm.height
            color: "#e7e6e6"
            opacity: 1

            MouseArea {
                id: mouseArea
                width: parent.width
                height: parent.height
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                anchors.fill: parent


                TabView {
                    id: tabview
                    x: 186
                    y: 75
                    width: mainForm.width - tabview.x - 5
                    height: 170
                    anchors {
                        bottom: top + 98
                        right: mainForm.width - 5
                    }

                    FileDialog {
                        id: fileDialog
                        title: "Please choose a file"
                        folder: shortcuts.home
                        nameFilters: ["(*.mp3)"]
                        selectMultiple: true
                        onAccepted: {
                            for (var i = 0; i < fileDialog.fileUrls.length; ++i){
                            console.log("You chose: " + fileDialog.fileUrls[i])
                            var path = fileDialog.fileUrls[i].toString();
                                    path = path.replace(/^(file:\/{2})/,"");
                                    var cleanPath = decodeURIComponent(path);
                                    console.log(cleanPath);
                                    _presenter.addToPlaylist(cleanPath)}
                        }
                        onRejected: {
                            console.log("Canceled")

                        }

                    }

                    Dialog {
                        id: songTextDialog
                        title: "Song Text"
                        modality: Qt.NonModal
                        contentItem:
                            Flickable {
                            id: songTextFlickable
                            implicitHeight: 500
                            implicitWidth: 400
                            clip: true
                            contentHeight: songText.contentHeight + 20
                                Item {
                                    id: contentItem
                                    x: 10
                                    y: 10
                                    Text{
                                        id: songText
                                        color: "#343434"
                                    }
                                }
                        }
                    }

                    Dialog {
                        id: editTagsDialog
                        title: "Edit Tags"
                        modality: Qt.NonModal
                        standardButtons: StandardButton.Save | StandardButton.Cancel
                        height: 150
                        width: 300
                        GridLayout {
                            columns: 2
                            width: 300
                            rowSpacing: 15
                            Label {
                                id: artistTagLabel
                                x: 20
                                y: 20
                                height: 23
                                text: "Artist:"
                            }

                            Rectangle {
                                id: artistTagEditBackground
                                x: artistTagLabel.x + artistTagLabel.width + 10
                                y: artistTagLabel.y
                                width: 210
                                height: 23
                                radius: 3
                                border.width: 1
                                border.color: "black"
                                color: "white"
                                TextEdit {
                                    id: artistTagEdit
                                    x: 5
                                    y: artistTagEditBackground.y + 2
                                    width: artistTagEditBackground.width
                                }
                             }
                            Label {
                                id: titleTagLabel
                                x: 20
                                y: 50
                                height: 23
                                text: "Title:"
                            }

                            Rectangle {
                                id: titleTagEditBackground
                                x: titleTagLabel.x + titleTagLabel.width + 10
                                y: titleTagLabel.y
                                width: 210
                                height: 23
                                radius: 3
                                border.width: 1
                                border.color: "black"
                                color: "white"
                                TextEdit {
                                    id: titleTagEdit
                                    x: 5
                                    y: 2
                                    width: titleTagEditBackground.width
                                }
                             }
                            Label {
                                id: albumTagLabel
                                x: 20
                                y: 50
                                height: 23
                                text: "Album:"
                            }

                            Rectangle {
                                id: albumTagEditBackground
                                x: albumTagLabel.x + albumTagLabel.width + 10
                                y: albumTagLabel.y
                                width: 210
                                height: 23
                                radius: 3
                                border.width: 1
                                border.color: "black"
                                color: "white"
                                TextEdit {
                                    id: albumTagEdit
                                    x: 5
                                    y: 2
                                    width: albumTagEditBackground.width
                                }
                             }
                        }
                        onAccepted: {
                            var artist = artistTagEdit.text;
                            var title = titleTagEdit.text;
                            var album = albumTagEdit.text;
                            _presenter.editTags(artist, title, album);
                        }
                    }

                    Tab {
                        id: tab1
                        objectName: "tab1"
                        title: "Review"
                        GridView{
                            id: gridView
                            objectName: "gridView"
                            Image {
                                id: trackTemplate
                                objectName: "trackTemplate"
                                source: ""
                                cache: false
                                asynchronous: true
                                width: 120
                                height: 120
                                sourceSize.height: 120
                                sourceSize.width: 120
                                transformOrigin: Item.Center
                                x: tab1.x + 8
                                y: tab1.y + 8
                            }
                            Label {
                                id: artistName
                                objectName: "artistName"
                                text: ""
                                style: Text.Normal
                                font.family: "Tahoma"
                                font.pointSize: 16
                                font.bold: true
                                color: "#343434"
                                onTextChanged: {artistName.update();}
                                font.italic: false
                                x: trackTemplate.x + trackTemplate.width + 30
                                y: 20
                            }

                            Label {
                                id: trackName
                                objectName: "trackName"
                                text: ""
                                font.pointSize: 14
                                color: "#343434"
                                onTextChanged: trackName.update();
                                font.italic: true
                                x: trackTemplate.x + trackTemplate.width + 30
                                y: artistName.y + artistName.height + 10
                            }

                            Label {
                                id: albumName
                                objectName: "albumName"
                                text: ""
                                onTextChanged: albumName.update();
                                font.italic: true
                                font.pointSize: 10
                                x: trackTemplate.x + trackTemplate.width + 30
                                y: trackName.y + trackName.height + 5
                                color: "#8d8d8d"
                            }
                        }
                    }

                    Tab {
                        id: tab2
                        objectName: "tab2"
                        title: "Info"
                        active: true
                            GridView{
                                id: infoGridView
                                objectName: "infoGridView"
                                Label {
                                    id: artistNameInfoLabel
                                    text: "Artist:"
                                    color: "#343434"
                                    x: infoGridView.x + 20
                                    y: infoGridView.y + 5
                                }
                                Label {
                                    id: artistNameInfo
                                    objectName: "artistNameInfo"
                                    text: ""
                                    font.italic: true
                                    font.family: "Tahoma"
                                    font.bold: true
                                    color: "#343434"
                                    x: infoGridView.x + 90
                                    y: artistNameInfoLabel.y
                                }
                                Label {
                                    id: trackNameInfoLabel
                                    text: "Title:"
                                    color: "#343434"
                                    x: artistNameInfoLabel.x
                                    y: artistNameInfoLabel.y + artistNameInfoLabel.height + 10
                                }
                                Label {
                                    id: trackNameInfo
                                    objectName: "trackNameInfo"
                                    text: ""
                                    font.italic: true
                                    font.family: "Tahoma"
                                    font.bold: true
                                    color: "#343434"
                                    x: infoGridView.x + 90
                                    y: trackNameInfoLabel.y
                                }
                                Label {
                                    id: albumNameInfoLabel
                                    text: "Album:"
                                    color: "#343434"
                                    x: artistNameInfoLabel.x
                                    y: trackNameInfoLabel.y + trackNameInfoLabel.height + 10
                                }
                                Label {
                                    id: albumNameInfo
                                    objectName: "albumNameInfo"
                                    text: ""
                                    font.italic: true
                                    font.family: "Tahoma"
                                    font.bold: true
                                    color: "#343434"
                                    x: infoGridView.x + 90
                                    y: albumNameInfoLabel.y
                                }
                                Label {
                                    id: yearInfoLabel
                                    text: "Year:"
                                    color: "#343434"
                                    x: artistNameInfoLabel.x
                                    y: albumNameInfoLabel.y + albumNameInfoLabel.height + 10
                                }
                                Label {
                                    id: yearInfo
                                    objectName: "yearInfo"
                                    text: ""
                                    font.italic: true
                                    font.family: "Tahoma"
                                    font.bold: true
                                    color: "#343434"
                                    x: infoGridView.x + 90
                                    y: yearInfoLabel.y
                                }
                                Label {
                                    id: genreInfoLabel
                                    text: "Genre:"
                                    color: "#343434"
                                    x: artistNameInfoLabel.x
                                    y: yearInfoLabel.y + yearInfoLabel.height + 10
                                }
                                Label {
                                    id: genreInfo
                                    objectName: "genreInfo"
                                    text: ""
                                    font.italic: true
                                    font.family: "Tahoma"
                                    font.bold: true
                                    color: "#343434"
                                    x: infoGridView.x + 90
                                    y: genreInfoLabel.y
                                }

                                Image {
                                    id: showTextButton
                                    x: infoGridView.x + infoGridView.width - 30
                                    y: infoGridView.y + infoGridView.height - 33
                                    width: 27
                                    height: 27
                                    sourceSize.height: 27
                                    sourceSize.width: 27
                                    source: "showTextButton.svg"
                                    MouseArea {
                                        width: showTextButton.width
                                        height: showTextButton.height
                                        onClicked: {songTextDialog.open();
                                          songText.text =  _presenter.getSongTextFromFile();
                                        }
                                    }
                                }

                                Image {
                                    id: editTagsDialogButton
                                    x:  infoGridView.x + infoGridView.width - 70
                                    y: showTextButton.y - 1
                                    width: 27
                                    height: 27
                                    sourceSize.height: 27
                                    sourceSize.width: 27
                                    source: "tagSettings.svg"
                                    MouseArea {
                                        width: editTagsDialogButton.width
                                        height: editTagsDialogButton.height
                                        onClicked: {editTagsDialog.open();
                                            artistTagEdit.text = artistNameInfo.text;
                                            titleTagEdit.text = trackNameInfo.text;
                                            albumTagEdit.text = albumNameInfo.text;
                                        }
                                    }
                                }

//                                Label {
//                                    id: bitRateInfoLabel
//                                    text: "Bit Rate:"
//                                    x: artistNameInfoLabel.x
//                                    y: genreInfoLabel.y + genreInfoLabel.height + 10
//                                }
//                                Label {
//                                    id: bitRateInfo
//                                    objectName: "bitRateInfo"
//                                    text: ""
//                                    font.italic: true
//                                    x: infoGridView.x + 90
//                                    y: bitRateInfoLabel.y
//                                }
                        }
                    }
                }

                Label {
                    id: currentPositionLabel
                    x: 299
                    objectName: "currentPositionLabel"
                    y: 49
                    width: 37
                    height: 17
                    text: qsTr("")
                    color: "#cbcbcb"
                    z: 1
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                }

                Label {
                    id: trackDurationLabel
                    x: 385
                    objectName: "trackDurationLabel"
                    y: 49
                    width: 37
                    text: qsTr("")
                    color: "#cbcbcb"
                    z: 2
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                Label {
                    id: estimatedDurationLabel
                    x: 466
                    objectName: "estimatedDurationLabel"
                    y: 49
                    width: 37
                    color: "#cbcbcb"
                    text: qsTr("")
                    z: 3
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                }

                Rectangle {
                    id: playArea
                    x: 0
                    y: 0
                    width: parent.width
                    height: 70
                    color: "#343434"

                    Row {
                        id: playRow
                        x: 46
                        y: 21
                        width: 160
                        height: 38
                        anchors.verticalCenterOffset: 0
                        anchors.verticalCenter: parent.verticalCenter
                        spacing: 20


                        Image {
                            id: previousTrackButton
                            width: 22
                            height: 22
                            sourceSize.height: 50
                            sourceSize.width: 50
                            fillMode: Image.Stretch
                            antialiasing: true
                            anchors.verticalCenter: parent.verticalCenter
                            source: "previousTrackIcon.svg"
                            MouseArea {
                                width: 35
                                height: 35
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: _presenter.goToPreviousTrack()
                            }
                        }

                        Image {
                            id: playButton
                            objectName: "playButton"
                            width: 35
                            height: 35
                            anchors.verticalCenter: parent.verticalCenter
                            sourceSize.height: 50
                            antialiasing: false
                            sourceSize.width: 50
                            smooth: true
                            signal playButtonClicked()
                            source: "playIcon.svg"
                            onSourceChanged: playButton.update()
                            MouseArea {
                                id: playButtonMouseArea
                                height: 45
                                width: 45
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.left: parent.left
                                anchors.bottom: parent.bottom
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.top: parent.top
                                onClicked: { _presenter.playButtonClicked();
                                }
                            }

                        }
                        Image {
                            id: nextTrackButton
                            width: 22
                            height: 22
                            sourceSize.height: 50
                            sourceSize.width: 50
                            antialiasing: true
                            smooth: true
                            anchors.verticalCenter: parent.verticalCenter
                            source: "nextTrackIcon.svg"
                            MouseArea {
                                width: 35
                                height: 35
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: _presenter.goToNextTrack()
                            }
                        }

                    }

                    Row {
                        id: extendToolsRow
                        x: 226
                        y: 21
                        width: 466
                        height: 64
                        spacing: 10
                        anchors.verticalCenter: parent.verticalCenter

                        Image {
                            id: repeatButton
                            objectName: "repeatButton"
                            width: 25
                            height: 25
                            anchors.horizontalCenter: mouseArea.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            antialiasing: true
                            sourceSize.height: 50
                            sourceSize.width: 50
                            source: "reload.svg"
                            MouseArea {
                                height: 35
                                width: 35
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: _presenter.repeatTrackButtonPressed()
                            }
                        }

                        Image {
                            id: randomShuffleButton
                            width: 25
                            height: 25
                            sourceSize.height: 35
                            sourceSize.width: 35
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: mouseArea.horizontalCenter
                            antialiasing: true
                            source: "randomPlayIcon.svg"
                            MouseArea {
                                height: 35
                                width: 35
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                                onClicked: _presenter.shuffleButtonPressed()
                            }
                        }

                        Slider {
                            id: trackSlider
                            objectName: "trackSlider"
                            width: 206
                            maximumValue: 100
                            stepSize: 0.5
                            anchors.verticalCenter: parent.verticalCenter
                            onValueChanged: {/*playMusic.source = "file:///home/alexey/11 - That's My Heart.mp3";*/
//                                console.debug(playMusic.source);
//                                console.debug(playMusic.metaData.coverArtUrlSmall);
                            }
                            onPressedChanged: {if (trackSlider.pressed == false) {console.debug(trackSlider.value);
                                    _presenter.positionChangedFromUser();}
                            }
                        }

                        Image {
                            id: muteButton
                            objectName: "muteButton"
                            width: 25
                            height: 25
                            sourceSize.height: 30
                            sourceSize.width: 30
                            anchors.verticalCenter: parent.verticalCenter
                            source: "speakerIcon.svg"
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    if (muteButton.source == "qrc:/speakerIcon.svg") {
                                        _presenter.mute(); muteButton.source = "qrc:/mutedIcon.svg"; muteButton.update();
                                    } else {
                                        _presenter.mute(); muteButton.source = "qrc:/speakerIcon.svg"; muteButton.update();
                                    }
                                }
                            }
                        }

                        Slider {
                            id: volumeSlider
                            objectName: "volumeSlider"
                            value: 50
                            width: 100
                            maximumValue: 100
                            stepSize: 1
                            anchors.verticalCenter: parent.verticalCenter
                            onValueChanged: { _presenter.volumeChanged();}
                        }


                    }
                }
                TableView{
                    id: playlistView
                    x: 10
                    y: 255
                    width: mainForm.width - playlistView.x - 5
                    height: mainForm.height - playlistView.y - 35
                    sortIndicatorVisible: true
                    TableViewColumn {
                        id: numberColumn
                        role: "role_number"
                        title: ""
                        width: 25
                    }
                    TableViewColumn {
                        role: "role_title"
                        title: "Title"
                        width: 150
                    }
                    TableViewColumn {
                        role: "role_artist"
                        title: "Artist"
                        width: 150
                    }
                    TableViewColumn {
                        role: "role_album"
                        title: "Album"
                        width: 200
                    }
                    model: _model
                }
            }
        }

        Image {
            id: addToPlaylistButton
            x: mainForm.width - 55
            y: 73
            width: 27
            height: 27
            sourceSize.height: 27
            sourceSize.width: 27
            source: "addToPlaylistIcon.svg"
            MouseArea {
                width: addToPlaylistButton.width
                height: addToPlaylistButton.height
                onClicked: fileDialog.open();
            }
        }
    }
}

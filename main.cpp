#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "presenter.h"
#include "filecontroller.h"
#include "player.h"
#include "playlisttablemodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));


    QObject* root = engine.rootObjects()[0];
    Presenter presenter(root);
    PlaylistTableModel model(&presenter);
    engine.rootContext()->setContextProperty("_presenter", &presenter);
    engine.rootContext()->setContextProperty("_model", &model);
//    FileController::getLyrics();


    return app.exec();
}


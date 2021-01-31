#include <QDebug>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QSqlDatabase>

#include <McIoc/ApplicationContext/impl/McAnnotationApplicationContext.h>
#include <McOrm/DaoLoader/impl/McDaoLoader.h>

#include "MusicDao.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(R"(..\..\Examples\OrmTest\d_orm_test.db)");
    qDebug() << db.open();
    
    McAnnotationApplicationContextPtr appCtx = McAnnotationApplicationContextPtr::create();
    appCtx->getBean<McDaoLoader>("daoLoader")->init(appCtx);

    auto musicDao = appCtx->getBean<MusicDao>("musicDao");
    auto music = musicDao->selectById(1);
    qDebug() << music->id << music->songTitle << music->songUrl << music->albumIndex
             << music->album->id << music->album->albumId << music->album->albumMid
             << music->album->albumName << music->album->albumTitle
             << music->album->albumTitleHilight;
    //    auto musics = musicDao->selectAll();
    //    qDebug() << musics;

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    
    return app.exec();
}

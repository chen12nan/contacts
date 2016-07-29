#include "utildb.h"
#include <QtSql>
#include <QDir>
#include <QDebug>
#include "db_global.h"

QSqlDatabase UtilDB::createDatabase(const QString &path, const QString &name, bool close)
{
    QDir databaseDir(path);
    if (!databaseDir.exists())
    {
        DEBUG()<<"DB DIR NOT EXIST";
        // create the path
        databaseDir.mkpath(QLatin1String("."));
    }
    const QString databaseFile = databaseDir.absoluteFilePath(name);
    QSqlDatabase database = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"), name);
    database.setDatabaseName(databaseFile);

    if (!database.open())
    {
        qWarning() << QString("Failed to open %1 database").arg(name);
        qWarning() << database.lastError();
    }
    QStringList schemas;
    //sqlite3开启了WAL
    schemas<<"PRAGMA temp_store = MEMORY"<<"PRAGMA journal_mode = WAL"<<"PRAGMA foreign_keys = ON";
     setDatabaseSchema(database, schemas);
     if(close)
         database.close();
    return database;
}
bool UtilDB::createTable(QSqlDatabase &db, const QStringList &schemas)
{
    if(!db.transaction())
    {
        DEBUG()<<"\033[31m"<<Q_FUNC_INFO<<"----fail to begin a transaction on the database\033[0m";
        return false;
    }
    QSqlQuery query(db);
    bool error = false;
    foreach(QString schema, schemas)
    {
        if(!query.exec(schema))
        {
            DEBUG()<<Q_FUNC_INFO<<__LINE__<<schema;
            qWarning()<<Q_FUNC_INFO<< query.lastError();
            error = true;
        }
    }
    if(error)
    {
        db.rollback();
        return false;
    }
    else
    {
        return db.commit();
    }
}

bool UtilDB::setDatabaseSchema(QSqlDatabase& db, const QStringList &schemas)
{
    QSqlQuery query(db);
    foreach(QString schema, schemas)
    {
        if (!query.exec(schema))
        {
            qWarning() << query.lastError();
            qWarning() << schema;
            return false;
        }
    }
    return true;
}

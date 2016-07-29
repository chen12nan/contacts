#ifndef UTILDB_H
#define UTILDB_H

#include <QtSql>

class UtilDB
{
public:
    static QSqlDatabase createDatabase(const QString& path, const QString& name, bool close = true);
    static bool createTable(QSqlDatabase& db, const QStringList& schemas);
    static bool setDatabaseSchema(QSqlDatabase& db, const QStringList& schemas);
};
#endif // UTILDB_H

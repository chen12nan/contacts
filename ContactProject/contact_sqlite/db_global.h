#ifndef CONTACTS_GLOBAL_H
#define CONTACTS_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QDebug>

#define CONTACT_DEBUG
#ifdef CONTACT_DEBUG
#define DEBUG qDebug
#else
#define DEBUG if (0) qDebug
#endif

#if defined(DB_LIBRARY)
#  define DB_EXPORT Q_DECL_EXPORT
#else
#  define DB_EXPORT Q_DECL_IMPORT
#endif


#endif // CONTACTS_GLOBAL_H

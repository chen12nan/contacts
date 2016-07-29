#ifndef CCONTACT_GLOBAL_H
#define CCONTACT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SYBERPIM_LIBRARY)
#  define SYBERPIM_EXPORT Q_DECL_EXPORT
#else
#  define SYBERPIM_EXPORT Q_DECL_IMPORT
#endif

#endif // CCONTACT_GLOBAL_H

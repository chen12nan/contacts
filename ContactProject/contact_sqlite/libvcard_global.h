
#ifndef VCARD_GLOBAL_H
#define VCARD_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QDebug>
#include "db_global.h"

#define VC_SEPARATOR_TOKEN ';'
#define VC_END_LINE_TOKEN '\n'
#define VC_BEGIN_TOKEN "BEGIN:VCARD"
#define VC_END_TOKEN "END:VCARD"
#define MAX_LINE_LENGTH 76
enum vCardVersion
{
    VC_VER_2_1,
    VC_VER_3_0
};

#endif // VCARD_GLOBAL_H

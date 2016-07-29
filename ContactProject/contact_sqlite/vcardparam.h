
#ifndef VCARDPARAM_H
#define VCARDPARAM_H

#include "libvcard_global.h"
#include <QtCore/QString>

class vCardParam
{
public:
    enum vCardParamGroup
    {
        Type,
        Encoding,
        Charset,
        Undefined
    };

protected:
    vCardParamGroup m_group;
    QString m_value;

public:
    vCardParam();
    vCardParam(const QString& value, vCardParamGroup group = vCardParam::Undefined);
    ~vCardParam();

    vCardParamGroup group() const;
    void setGroup(vCardParamGroup group);
    QString value() const;
    void setValue(const QString &value);

    bool isValid() const;

    bool operator== (const vCardParam& param) const;
    bool operator!= (const vCardParam& param) const;

    QByteArray toByteArray(vCardVersion version = VC_VER_3_0) const;

    static QByteArray toByteArray(QList<vCardParam> params, vCardVersion version = VC_VER_3_0);
    static QList<vCardParam> fromByteArray(const QByteArray& data);
};

typedef QList<vCardParam> vCardParamList;

#endif // VCARDPARAM_H

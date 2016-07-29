
#include "vcardproperty.h"
#include "stdio.h"
#include <QTextCodec>

#define VC_ASSIGNMENT_TOKEN ':'

vCardProperty::vCardProperty()
{
}

vCardProperty::vCardProperty(const QString& name, const QString& value, const vCardParamList& params)
    :   m_name(name),
        m_params(params)
{
    m_values = value.split(VC_SEPARATOR_TOKEN);
}

vCardProperty::vCardProperty(const QString& name, const QStringList& values, const vCardParamList& params)
    :   m_name(name),
        m_values(values),
        m_params(params)
{
}

vCardProperty::vCardProperty(const QString& name, const QString& value, const QString& params)
    :   m_name(name)
{
    m_values = value.split(VC_SEPARATOR_TOKEN);
    m_params = vCardParam::fromByteArray(params.toUtf8());
}

vCardProperty::vCardProperty(const QString& name, const QStringList& values, const QString& params)
    :   m_name(name),
        m_values(values)
{
    m_params = vCardParam::fromByteArray(params.toUtf8());
}

vCardProperty::~vCardProperty()
{
}

QString vCardProperty::name() const
{
    return m_name;
}

QString vCardProperty::value() const
{
    return m_values.join(QString(VC_SEPARATOR_TOKEN));
}

QStringList vCardProperty::values() const
{
    return m_values;
}

vCardParamList vCardProperty::params() const
{
    return m_params;
}

bool vCardProperty::isValid() const
{
    if (m_name.isEmpty())
        return false;

    if (m_values.isEmpty())
        return false;

    foreach (vCardParam param, m_params)
        if (!param.isValid())
            return false;

    return true;
}

bool vCardProperty::operator== (const vCardProperty& prop) const
{
    return ((m_name == prop.name()) && (m_values == prop.values()));
}

bool vCardProperty::operator!= (const vCardProperty& prop) const
{
    return ((m_name != prop.name()) || (m_values != prop.values()));
}

QByteArray vCardProperty::toByteArray(vCardVersion version) const
{
    QByteArray buffer;

    switch (version)
    {
        case VC_VER_2_1:
        case VC_VER_3_0:
        {
            buffer.append(m_name).toUpper();
            QByteArray params = vCardParam::toByteArray(m_params, version);

            if (!params.isEmpty())
            {
                buffer.append(VC_SEPARATOR_TOKEN);
                buffer.append(params);
            }

            buffer.append(QString(VC_ASSIGNMENT_TOKEN));
            buffer.append(m_values.join(QString(VC_SEPARATOR_TOKEN)));
        }
        break;

        default:
            break;
    }

    return buffer;
}

static QString decode(const QString &input)
{
    // 0 1 2 3 4 5 6 7 8 9 : ; < = > ? @ A B C D E F
    const int hexVal[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 10, 11, 12, 13, 14, 15};
    QByteArray output;

    for (int i = 0; i < input.length(); ++i)
    {
        if (input.at(i).toLatin1() == '=')
        {
            output.append((hexVal[input.at(++i).toLatin1() - '0'] << 4) + hexVal[input.at(++i).toLatin1() - '0']);
        }
        else
        {
            output.append(input.at(i).toLatin1());
        }
    }
    QTextCodec *codec = QTextCodec::codecForName("GB2312");
    return codec->toUnicode(output);
}

QList<vCardProperty> vCardProperty::fromByteArray(const QByteArray& data)
{
    QList<vCardProperty> properties;

    QStringList lines = QString::fromUtf8(data).split(VC_END_LINE_TOKEN);
    foreach (QString line, lines)
    {
        if (line == VC_BEGIN_TOKEN || line == VC_END_TOKEN)
            break;

        QStringList tokens = line.split(VC_ASSIGNMENT_TOKEN);
        if (tokens.count() >= 2)
        {
            QStringList property_tokens = tokens.at(0).split(VC_SEPARATOR_TOKEN);
            QString name = property_tokens.takeAt(0);

            if (name != VC_VERSION)
            {
                vCardParamList params = vCardParam::fromByteArray(property_tokens.join(QString(VC_SEPARATOR_TOKEN)).toUtf8());

                DEBUG() << "name:" << name << " tokens.at(1):" << tokens.at(1);
                QString decodeStr = tokens.at(1);
                DEBUG() << "decodeStr:" << __LINE__ << decodeStr;
                for (int i = 0; i < params.size(); ++i) {
                    if (params.at(i).group() == vCardParam::Encoding && params.at(i).value() == "QUOTED-PRINTABLE") {
                        decodeStr = decode(decodeStr);
                        DEBUG() << "=============decodeStr:" << __LINE__ << decodeStr;
                        break;
                    }
                }
                properties.append(vCardProperty(name, decodeStr, params));
            }
        }
    }

    return properties;
}

vCardProperty vCardProperty::createAddress(const QString& street, const QString& locality, const QString& region, const QString& postal_code, const QString& country, const QString& post_office_box, const QString& ext_address, const vCardParamList& params)
{
    QStringList values;
    values.insert(vCardProperty::PostOfficeBox, post_office_box);
    values.insert(vCardProperty::ExtendedAddress, ext_address);
    values.insert(vCardProperty::Street, street);
    values.insert(vCardProperty::Locality, locality);
    values.insert(vCardProperty::Region, region);
    values.insert(vCardProperty::PostalCode, postal_code);
    values.insert(vCardProperty::Country, country);

    return vCardProperty(VC_ADDRESS, values, params);
}

vCardProperty vCardProperty::createBirthday(const QDate& birthday, const vCardParamList& params)
{
    return vCardProperty(VC_BIRTHDAY, birthday.toString("yyyy-MM-dd"), params);
}

vCardProperty vCardProperty::createBirthday(const QDateTime& birthday, const vCardParamList& params)
{
    return vCardProperty(VC_BIRTHDAY, birthday.toString("yyyy-MM-dd"), params);
}

vCardProperty vCardProperty::createGeographicPosition(qreal latitude, qreal longitude, const vCardParamList& params)
{
    QStringList values;
    values.insert(vCardProperty::Latitude, QString("%1").arg(latitude));
    values.insert(vCardProperty::Longitude, QString("%1").arg(longitude));

    return vCardProperty(VC_GEOGRAPHIC_POSITION, values, params);
}

vCardProperty vCardProperty::createName(const QString& firstname, const QString& lastname, const QString& additional, const QString& prefix, const QString& suffix, const vCardParamList& params)
{
    QStringList values;
    values.insert(vCardProperty::Lastname, lastname);
    values.insert(vCardProperty::Firstname, firstname);
    values.insert(vCardProperty::Additional, additional);
    values.insert(vCardProperty::Prefix, prefix);
    values.insert(vCardProperty::Suffix, suffix);

    return vCardProperty(VC_NAME, values, params);
}

vCardProperty vCardProperty::createOrganization(const QString& name, const QStringList& levels, const vCardParamList& params)
{
    QStringList values;
    values.append(name);
    values.append(levels);

    return vCardProperty(VC_ORGANIZATION, values, params);
}


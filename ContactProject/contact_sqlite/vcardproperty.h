
#ifndef VCARDPROPERTY_H
#define VCARDPROPERTY_H

#include "vcardparam.h"
#include <QtCore/QDateTime>
#include <QtCore/QStringList>

#define VC_ADDRESS              "ADR"
#define VC_AGENT                "AGENT"
#define VC_BIRTHDAY             "BDAY"
#define VC_CATEGORIES           "CATEGORIES"
#define VC_CLASS                "CLASS"
#define VC_DELIVERY_LABEL       "LABEL"
#define VC_EMAIL                "EMAIL"
#define VC_FORMATTED_NAME       "FN"
#define VC_GEOGRAPHIC_POSITION  "GEO"
#define VC_KEY                  "KEY"
#define VC_LOGO                 "LOGO"
#define VC_MAILER               "MAILER"
#define VC_NAME                 "N"
#define VC_NICKNAME             "NICKNAME"
#define VC_NOTE                 "NOTE"
#define VC_ORGANIZATION         "ORG"
#define VC_PHOTO                "PHOTO"
#define VC_PRODUCT_IDENTIFIER   "PRODID"
#define VC_REVISION             "REV"
#define VC_ROLE                 "ROLE"
#define VC_SORT_STRING          "SORT-STRING"
#define VC_SOUND                "SOUND"
#define VC_TELEPHONE            "TEL"
#define VC_TIME_ZONE            "TZ"
#define VC_TITLE                "TITLE"
#define VC_URL                  "URL"
#define VC_VERSION              "VERSION"
#define VC_CUSTOM_ADDRESS       "X-ADDRESS-CUSTOM-SKYTREE"
#define VC_CUSTOM_EMAIL         "X-EMAIL-GENERAL-SKYTREE"
#define VC_CUSTOM_TEL           "X-TEL-CUSTOM-SKYTREE"
#define VC_CUSTOM_URL           "X-URL-CUSTOM-SKYTREE"
#define VC_CUSTOM_FAVORITE      "X-QTPROJECT-FAVORITE"
#define VC_CUSTOM_GENDER        "X-GENDER"
#define VC_CUSTOM_GROUP         "X-GROUP"


class vCardProperty
{
public:
    enum GenericFields
    {
        DefaultValue = 0
    };

    enum AddressFields
    {
        PostOfficeBox = 0,
        ExtendedAddress,
        Street,
        Locality,           // e.g. City.
        Region,             // e.g. State or province.
        PostalCode,         //
        Country
    };

    enum NameFields
    {
        Lastname = 0,
        Firstname,
        Additional,
        Prefix,
        Suffix
    };

    enum GeographicPositionFields
    {
        Latitude = 0,
        Longitude
    };

protected:
    QString m_name;
    QStringList m_values;
    vCardParamList m_params;

public:
    vCardProperty();
    vCardProperty(const QString& name, const QString& value, const vCardParamList& params = vCardParamList());
    vCardProperty(const QString& name, const QStringList& values, const vCardParamList& params = vCardParamList());
    vCardProperty(const QString& name, const QString& value, const QString& params);
    vCardProperty(const QString& name, const QStringList& values, const QString& params);
    ~vCardProperty();

    QString name() const;
    QString value() const;
    QStringList values() const;
    vCardParamList params() const;
    bool isValid() const;

    bool operator== (const vCardProperty& param) const;
    bool operator!= (const vCardProperty& param) const;

    QByteArray toByteArray(vCardVersion version = VC_VER_3_0) const;

    static QList<vCardProperty> fromByteArray(const QByteArray& data);

    static vCardProperty createAddress(const QString& street, const QString& locality, const QString& region, const QString& postal_code, const QString& country, const QString& post_office_box = "", const QString& ext_address = "", const vCardParamList& params = vCardParamList());
    static vCardProperty createBirthday(const QDate& birthday, const vCardParamList& params = vCardParamList());
    static vCardProperty createBirthday(const QDateTime& birthday, const vCardParamList& params = vCardParamList());
    static vCardProperty createGeographicPosition(qreal latitude, qreal longitude, const vCardParamList& params = vCardParamList());
    static vCardProperty createName(const QString& firstname, const QString& lastname, const QString& additional = "", const QString& prefix = "", const QString& suffix = "", const vCardParamList& params = vCardParamList());
    static vCardProperty createOrganization(const QString& name, const QStringList& levels = QStringList(), const vCardParamList& params = vCardParamList());
};

typedef QList<vCardProperty> vCardPropertyList;

#endif // VCARDPROPERTY_H

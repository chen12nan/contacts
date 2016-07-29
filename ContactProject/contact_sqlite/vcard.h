#ifndef VCARD_H
#define VCARD_H

#include "vcardproperty.h"

class vCard
{
protected:
    vCardPropertyList m_properties;

public:
    vCard();
    vCard(const vCard& vcard);
    vCard(const vCardPropertyList& properties);
    virtual ~vCard();
    
    void addProperty(const vCardProperty& property);
    void removeProperty(const vCardProperty& property);
    void addProperties(const vCardPropertyList& properties);
    vCardPropertyList properties() const;
    vCardProperty property(const QString& name, const vCardParamList& params = vCardParamList(), bool strict = false) const;
    bool contains(const QString& property, const vCardParamList& params = vCardParamList(), bool strict = false) const;
    bool contains(const vCardProperty& property) const;
    bool isValid() const;
    void clear();
    
    int count() const;
    QByteArray toByteArray(vCardVersion version = VC_VER_3_0) const;
    
    bool saveToFile(const QString& filename) const;

    static QList<vCard> fromByteArray(const QByteArray& data);
    static QList<vCard> fromFile(const QString& filename);
    static bool saveToFile(const QString& filename, const QList<vCard> &vcards);
};

typedef QList<vCard> vCardList;

#endif // VCARD_H

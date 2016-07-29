#ifdef LOCAL_DEBUG
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#endif
#include "ccontactdb.h"
#include "utildb.h"
#include <ccontact.h>
#include "vcard.h"

#define CONTACT_DATABASE_DIR "/data/pim"
#define CONTACT_DATABASE_NAME "contacts.db"

static const char *db_setup[] =
{
    "PRAGMA temp_store = MEMORY",
    "PRAGMA journal_mode = WAL",
    "PRAGMA foreign_keys = ON"
};

static int db_setup_count = sizeof(db_setup) / sizeof(*db_setup);

static const char *contacts_schema[] =
{
    "PRAGMA encoding = \"UTF-8\"",
    "CREATE TABLE Contacts ( "
    "contactId INTEGER PRIMARY KEY ASC AUTOINCREMENT,"
    "groupId INTEGER KEY,"
    "displayLabel TEXT,"
    "firstName TEXT,"
    "lowerFirstName TEXT,"
    "lastName TEXT,"
    "lowerLastName TEXT,"
    "middleName TEXT,"
    "organization TEXT,"
    "gender TEXT,"
    "avatar TEXT,"
    "ringtone TEXT,"
    "syncTarget TEXT NOT NULL,"
    "note TEXT,"
    "pingyinName TEXT,"
    "lowerPingyinName TEXT,"
    "t9Index TEXT,"
    "birthday DATETIME,"
    "created DATETIME,"
    "modified DATETIME,"
    "mdmId INTEGER,"
    "calendarId INTEGER,"
    "isFavorite BOOL,"
    "hasPhoneNumber BOOL,"
    "hasEmailAddresses BOOL,"
    "hasUrls BOOL,"
    "hasAddresses BOOL"
    ")"
};
// if type="other" read customText
static const char *groups_schema[] =
{
    "PRAGMA encoding = \"UTF-8\"",
    "CREATE TABLE Groups ( "
    "groupId INTEGER PRIMARY KEY ASC AUTOINCREMENT,"
    "contactIds TEXT,"
    "description TEXT,"
    "type TEXT,"
    "customText TEXT"
    ")"
};

static const char *phonenumbers_schema[] =
{
    "PRAGMA encoding = \"UTF-8\"",
    "CREATE TABLE PhoneNumbers ( "
    "detailId INTEGER PRIMARY KEY ASC AUTOINCREMENT,"
    "contactId INTEGER KEY,"
    "phoneNumber TEXT,"
    "normalizedNumber TEXT,"
    "type TEXT,"
    "customText TEXT"
    ")"
};

static const char *urls_schema[] =
{
    "PRAGMA encoding = \"UTF-8\"",
    "CREATE TABLE Urls ( "
    "detailId INTEGER PRIMARY KEY ASC AUTOINCREMENT,"
    "contactId INTEGER KEY,"
    "url TEXT,"
    "type TEXT,"
    "customText TEXT"
    ")"
};

static const char *emailAddresses_schema[] =
{
    "PRAGMA encoding = \"UTF-8\"",
    "CREATE TABLE EmailAddresses ( "
    "detailId INTEGER PRIMARY KEY ASC AUTOINCREMENT,"
    "contactId INTEGER KEY,"
    "emailAddress TEXT,"
    "lowerEmailAddress TEXT,"
    "type TEXT,"
    "customText TEXT"
    ")"
};

static const char *addresses_schema[] =
{
    "PRAGMA encoding = \"UTF-8\"",
    "CREATE TABLE Addresses ( "
    "detailId INTEGER PRIMARY KEY ASC AUTOINCREMENT,"
    "contactId INTEGER KEY,"
    "street TEXT,"
    "postOfficeBox TEXT,"
    "region TEXT,"
    "locality TEXT,"
    "postCode TEXT,"
    "country TEXT,"
    "type TEXT,"
    "customText TEXT"
    ")"
};
static const char* sqlcmd[] =
{
    "\n SELECT Contacts.contactId,"
    "\n Contacts.displayLabel,"
    "\n Contacts.firstName,"
    "\n Contacts.lowerFirstName,"
    "\n Contacts.lastName,"
    "\n Contacts.lowerLastName, "
    "\n Contacts.middleName,"
    "\n Contacts.organization,"
    "\n Contacts.gender,"
    "\n Contacts.avatar,"
    "\n Contacts.ringtone,"
    "\n Contacts.syncTarget,"
    "\n Contacts.note,"
    "\n Contacts.pingyinName,"
    "\n Contacts.lowerPingyinName,"
    "\n Contacts.t9Index,"
    "\n Contacts.birthday,"
    "\n Contacts.created,"
    "\n Contacts.modified,"
    "\n Contacts.mdmId,"
    "\n Contacts.calendarid,"
    "\n Contacts.isFavorite,"
    "\n PN.pns, PN.pnTypes, URL.urls, URL.urlTypes, EM.eas, EM.eaTypes, AD.streets, AD.addTypes from Contacts "
    "left join (select contactId, GROUP_CONCAT(phoneNumber) as pns, GROUP_CONCAT(type) as pnTypes from PhoneNumbers where contactId=%1) as PN "
    "left join (select contactId, GROUP_CONCAT(url) as urls, GROUP_CONCAT(type) as urlTypes from Urls where contactId=%2) as URL "
    "left join (select contactId, GROUP_CONCAT(emailAddress) as eas, GROUP_CONCAT(type) as eaTypes from EmailAddresses where contactId=%3) as EM "
    "left join (select contactId, GROUP_CONCAT(street) as streets, GROUP_CONCAT(type) as addTypes from Addresses where contactId=%4) as AD  where Contacts.contactId=%5;"
};

static QHash<int,QString> map;
void initPinyinMap() {
    QFile file(":/unicode_to_hanyu_pinyin.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        bool ok;
        map.insert(line.split(" ").at(0).toInt(&ok, 16), line.split(" ").at(1));
    }
    file.close();
}
QString getPinyinString(QString in)
{
    QChar *data = in.data();
    ushort findkey = data->unicode();
    QString value = map[findkey];
    if (value.contains(",")) {
        value = value.split(",").at(0);
    }
    return value;

}
QString convert(QString str) {
    QString result;
    result = getPinyinString(str);
    if (result.isEmpty()){
        result = QString(str).toUpper();
    }
    if (result.toUpper().at(0).isLetter() || result.toUpper().at(0).isSymbol())
        return result.toUpper();
    else
        return "#";
}
QChar getPinyinChar(QString in)
{
    if (in.isEmpty())
        return QChar('#');
    QString out = getPinyinString(in);
    if (out.isEmpty()) {
        if (in.at(0).isLetter())
            return in.at(0).toUpper();
        else
            return QChar('#');
    }
    return out.at(0).toUpper();

}
QString getNumFromPinyin(QString str) {
    // TODO Auto-generated method stub
    QString returnNumber = "";
    for (int i=0; i<str.length(); i++){
        int num = str.toLocal8Bit().at(i);
        switch (num) {
        case 'A':
        case 'B':
        case 'C':
            returnNumber.append('2');
            break;
        case 'D':
        case 'E':
        case 'F':
            returnNumber.append('3');
            break;
        case 'G':
        case 'H':
        case 'I':
            returnNumber.append('4');
            break;
        case 'J':
        case 'K':
        case 'L':
            returnNumber.append('5');
            break;
        case 'M':
        case 'N':
        case 'O':
            returnNumber.append('6');
            break;
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
            returnNumber.append('7');
            break;
        case 'T':
        case 'U':
        case 'V':
            returnNumber.append('8');
            break;
        case 'W':
        case 'X':
        case 'Y':
        case 'Z':
            returnNumber.append('9');
            break;
        default:
            returnNumber.append("");
            break;
        }
    }
    return returnNumber;
}

QStringList dealVCardRecord(const QString& record)
{
    QStringList fields;
    QString tmp;
    for(int i = 0; i<record.count(); i++)
    {
        QString s = record.mid(i,1);
        if(s == ";" || s == ":")
        {
            fields.append(tmp);
            fields.append(s);
            tmp.clear();
        }
        else
        {
            tmp.append(record[i]);
        }
    }

    fields.append(tmp);
    return fields;
}

CContactDB* CContactDB::_instance = NULL;
CContactDB::ARC  CContactDB::arc;


const char *queryContacts =
        "\nSELECT "
        "\nContacts.contactId, "
        "\nContacts.displayLabel, "
        "\nContacts.firstName, "
        "\nContacts.lowerFirstName,"
        "\nContacts.lastName, "
        "\nContacts.lowerLastName, "
        "\nContacts.middleName, "
        "\nContacts.organization, "
        "\nContacts.gender, "
        "\nContacts.avatar, "
        "\nContacts.ringtone, "
        "\nContacts.syncTarget, "
        "\nContacts.note, "
        "\nContacts.pingyinName, "
        "\nContacts.lowerPingyinName, "
        "\nContacts.t9Index, "
        "\nContacts.birthday, "
        "\nContacts.created, "
        "\nContacts.modified, "
        "\nContacts.mdmId, "
        "\nContacts.calendarid, "
        "\nContacts.isFavorite, "
        "\nPN.pns, PN.pnTypes, "
        "\nURL.urls, URL.urlTypes, "
        "\nEM.eas, EM.eaTypes, "
        "\nAD.streets, AD.addTypes "
        "\nfrom Contacts left join "
        "\n(select contactId, GROUP_CONCAT(phoneNumber) as pns, GROUP_CONCAT(type) as pnTypes from PhoneNumbers GROUP BY contactId) as PN ON (PN.contactId=Contacts.contactId) left join "
        "\n(select contactId, GROUP_CONCAT(url) as urls, GROUP_CONCAT(type) as urlTypes from Urls GROUP BY contactId) as URL ON (URL.contactId=Contacts.contactId) left join "
        "\n(select contactId, GROUP_CONCAT(emailAddress) as eas, GROUP_CONCAT(type) as eaTypes from EmailAddresses GROUP BY contactId) as EM ON (EM.contactId=Contacts.contactId) left join "
        "\n(select contactId, GROUP_CONCAT(street) as streets, GROUP_CONCAT(type) as addTypes from Addresses GROUP BY contactId) as AD ON (AD.contactId=Contacts.contactID) "
        "\nORDER BY Contacts.displayLabel ASC";

void CContactDB::readContactData(QSqlQuery &query, CContact &contact) {
    QString pns, pnTypes, urls, urlTypes, eas, eaTypes, streets, addTypes;

    contact.setId(query.value(0).toUInt());
    contact.setDisplayLabel(query.value(1).toString());
    contact.setFirstName(query.value(2).toString());
    contact.setLowerFirstName(query.value(3).toString());
    contact.setLastName(query.value(4).toString());
    contact.setLowerLastName(query.value(5).toString());
    contact.setMiddleName(query.value(6).toString());
    contact.setOrganization(query.value(7).toString());
    contact.setGender(query.value(8).toString());
    contact.setAvatar(query.value(9).toString());
    contact.setRingtone(query.value(10).toString());
    contact.setSyncTarget(query.value(11).toString());
    contact.setNote(query.value(12).toString());
    contact.setPingyinName(query.value(13).toString());
    contact.setLowerPingyinName(query.value(14).toString());
    contact.setT9Index(query.value(15).toString());
    contact.setBirthday(query.value(16).toDate());
    contact.setCreated(query.value(17).toDateTime());
    contact.setModified(query.value(18).toDateTime());
    contact.setMdmId(query.value(19).toUInt());
    contact.setCalendarId(query.value(20).toUInt());
    contact.setIsFavorite(query.value(21).toBool());
    pns = query.value(22).toString();
    pnTypes = query.value(23).toString();
    urls = query.value(24).toString();
    urlTypes = query.value(25).toString();
    eas = query.value(26).toString();
    eaTypes = query.value(27).toString();
    streets = query.value(28).toString();
    addTypes = query.value(29).toString();
    QStringList pnItems = pns.split(",");
    QStringList pnTypeItems = pnTypes.split(",");
    int count = qMin(pnItems.size(), pnTypeItems.size());
    for (int i = 0; i < count; i++)
    {
        contact.setPhoneNumber(qMakePair(pnTypeItems[i], pnItems[i]));
    }
    if (pnItems.size() > pnTypeItems.size()) {
        for (int i = count; i < pnItems.size(); i++)
        {
            contact.setPhoneNumber(qMakePair(QString("OTHER"), pnItems[i]));
        }
    }

    QStringList urlItems = urls.split(",");
    QStringList urlTypeItems = urlTypes.split(",");
    count = qMin(urlItems.size(), urlTypeItems.size());
    for (int i = 0; i < count; i++)
    {
        contact.setUrl(qMakePair(urlTypeItems[i], urlItems[i]));
    }
    if (urlItems.size() > urlTypeItems.size()) {
        for (int i = count; i < urlItems.size(); i++)
        {
            contact.setUrl(qMakePair(QString("OTHER"), urlItems[i]));
        }
    }

    QStringList eaItems = eas.split(",");
    QStringList eaTypeItems = eaTypes.split(",");
    count = qMin(eaItems.size(), eaTypeItems.size());
    for (int i = 0; i < count; i++)
    {
        contact.setEmailAddress(qMakePair(eaTypeItems[i], eaItems[i]));
    }
    if (eaItems.size() > eaTypeItems.size()) {
        for (int i = count; i < eaItems.size(); i++)
        {
            contact.setEmailAddress(qMakePair(QString("OTHER"), eaItems[i]));
        }
    }

    QStringList streetItems = streets.split(",");
    QStringList addTypeItems = addTypes.split(",");
    count = qMin(streetItems.size(), addTypeItems.size());
    for (int i = 0; i < count; i++)
    {
        contact.setAddress(qMakePair(addTypeItems[i], streetItems[i]));
    }
    if (streetItems.size() > addTypeItems.size()) {
        for (int i = count; i < streetItems.size(); i++)
        {
            contact.setAddress(qMakePair(QString("OTHER"), streetItems[i]));
        }
    }
}

QList<CContact> CContactDB::getContacts()
{
    QList<CContact> contacts;
    QSqlQuery query(m_database);
    QString cmd(queryContacts);

    if (!query.exec(cmd))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        DEBUG()<<Q_FUNC_INFO<<cmd;
        return contacts;
    }
    while (query.next())
    {
        CContact contact;
        readContactData(query, contact);
        contacts.append(contact);
    }

    return contacts;
}

bool CContactDB::deleteContact(uint id)
{
    QList<uint> ids;
    ids.push_back(id);
    return deleteContacts(ids);
}

bool CContactDB::saveContact(CContact contact)
{
    return saveContacts(QList<CContact>() << contact);
}

bool CContactDB::innerDeleteContact(uint id)
{
    if(id == 0)
    {
        DEBUG()<<"The Contact ID = 0";
        return false;
    }
    QStringList tables = m_database.tables();
    QSqlQuery query(m_database);
    foreach(QString tableName, tables)
    {
        if(tableName == "sqlite_sequence")
            continue;
        QString cmd = QString("delete from %1 where contactId = %2").arg(tableName).arg(id);
        if(!query.exec(cmd))
        {
            DEBUG()<<Q_FUNC_INFO<<query.lastError();
            DEBUG()<<Q_FUNC_INFO<<tableName;
            return false;
        }
    }
    return true;
}

bool CContactDB::deleteContacts(const QList<uint> &ids)
{
    if(!m_database.transaction())
    {
        DEBUG()<<Q_FUNC_INFO<<"fail to begin a transaction";
        return false;
    }
    bool error = false;
    foreach(uint id, ids)
    {
        error = innerDeleteContact(id);
        if(!error)
        {
            m_database.rollback();
            return false;
        }
    }
    return m_database.commit();
}
void CContactDB::saveNameDetails(CContact &contact, const QString &name) {
    QString firstName = name;
    QString pinyinName;
    QString t9Index;
    QString simplePinyin;
    QStringList tempList;
    QString displayLabel;
    int len = 0;
    if (firstName.length() > 0) {
        for (int i = 0; i < firstName.length(); i++) {
            DEBUG() << "firstName[" << i << "]: " << firstName[i];
            QString convStr;
            convStr = convert(QString(firstName[i]));
            DEBUG() << "convert: " << convStr;
            pinyinName += convStr;
            DEBUG() << "pinyinName: " << pinyinName;
            QString character = getNumFromPinyin(convStr);
            DEBUG() << "character: " << character;
            t9Index += character;
            DEBUG() << "pinyinNumName: " << t9Index;
            if (character != "")
                simplePinyin += character.at(0);
            else
                simplePinyin += "";
            if (i != firstName.length() -1) {
                len += character.length();
                tempList.append(QString::number(len));
            }
        }
    }
    tempList.prepend(t9Index);
    tempList.prepend(simplePinyin);
    t9Index = tempList.join(",");
    DEBUG() << "t9index: " << t9Index;
    if (pinyinName.length() > 0) {
        DEBUG() << "pinyinName: " << pinyinName;
        contact.setPingyinName(pinyinName);
        contact.setLowerPingyinName(pinyinName.toLower());
        //contact.setLastName(pinyinName);
    }
    else {
        DEBUG() << "pinyinName: #" ;
        contact.setPingyinName("#");
    }
    DEBUG() << "firstName" << firstName;

    QStringList nums = t9Index.split(",");
    displayLabel = pinyinName;
    if (nums.size() > 2) {
        displayLabel.insert(nums[2].trimmed().toInt(), " ");
    }
    displayLabel.append(" ").append(firstName);
    DEBUG() << "displayLabel: " << displayLabel;
    contact.setDisplayLabel(displayLabel);
    contact.setT9Index(t9Index);
    //contact.setMiddleName(pinyinNumName);
}

bool CContactDB::innerSaveContact(CContact &contact)
{
    saveNameDetails(contact, contact.firstName());
    if(contact.id() != 0)
    {
        DEBUG()<<"the contact has exists";
        return false;
    }
    QSqlQuery query(m_database);
    QString schema = QString("insert into Contacts(displayLabel, firstName, lowerFirstName,"
                          "lastName, lowerLastName, middleName, organization, gender, avatar,"
                          "ringtone, syncTarget, note, pingyinName, lowerPingyinName, t9Index,"
                          "birthday, created, modified, mdmId, calendarId, isFavorite,"
                          "hasPhoneNumber, hasEmailAddresses,hasUrls, hasAddresses)"
                          " values('%1','%2','%3','%4','%5','%6','%7','%8','%9',"
                          "'%10','%11','%12','%13','%14','%15','%16','%17','%18','%19','%20',"
                          "'%21','%22','%23','%24','%25')").arg(contact.displayLabel()).arg(contact.firstName())
            .arg(contact.lowerFirstName()).arg(contact.lastName()).arg(contact.lowerLastName())
            .arg(contact.middleName()).arg(contact.organization()).arg(contact.gender())
            .arg(contact.avatar()).arg(contact.ringtone()).arg(contact.syncTarget()).arg(contact.note())
            .arg(contact.pingyinName()).arg(contact.lowerPingyinName()).arg(contact.t9Index())
            .arg(contact.birthday().toString("yyyy-MM-dd")).arg(contact.created().toString("yyyy-MM-dd"))
            .arg(contact.modified().toString("yyyy-MM-dd")).arg(contact.mdmId())
            .arg(contact.calendarId()).arg(contact.isFavorite()).arg(contact.hasPhoneNumber())
            .arg(contact.hasEmailAddresses()).arg(contact.hasUrls()).arg(contact.hasAddresses());

    if(!query.exec(schema))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        return false;
    }
    contact.setId(query.lastInsertId().toUInt());
    if(!savePhoneNumbers(contact))
        return false;
    if(!saveEmailAddresses(contact))
        return false;
    if(!saveAddresses(contact))
        return false;
    if(!saveUrls(contact))
        return false;

    return true;
}

bool CContactDB::saveContacts(QList<CContact> &contacts)
{
    if(!m_database.transaction())
    {
        DEBUG()<<Q_FUNC_INFO<<"fail to begin a transaction";
        return false;
    }
    bool error = false;
    for(int i = 0; i < contacts.size(); ++i)
    {
        error = innerSaveContact(contacts[i]);
        if(!error)
        {
            m_database.rollback();
            return false;
        }
    }
    return m_database.commit();
}

bool CContactDB::updateContacts(QList<CContact> &contacts)
{
    if (!m_database.transaction())
    {
        DEBUG()<<Q_FUNC_INFO<<"fail to begin a transaction";
        return false;
    }
    bool error = false;
    for (int i = 0; i < contacts.size(); ++i)
    {
        error = updateContact(contacts[i]);
        if (!error)
        {
            m_database.rollback();
            return false;
        }
    }
    return m_database.commit();
}

bool CContactDB::updateContact(CContact &contact)
{
    return updateContactRecord(contact);
}

CContactDB *CContactDB::instance()
{
    if(!_instance)
    {
        _instance = new CContactDB();
    }
    return _instance;
}

static QString getParamValueByType(const vCardParamList &paramList, vCardParam::vCardParamGroup type) {
    QStringList value;
    foreach (const vCardParam &param, paramList) {
        if (param.group() == type) {
            value.append(param.value());
        }
    }
    return value.size() > 0 ? value.join("/") : QString();
}

static QString getAddressString(const QStringList &addrValues) {
    if (addrValues.size() != vCardProperty::Country + 1) {
        return addrValues.join("");
    }
    else {
        return addrValues[vCardProperty::Country] +
                addrValues[vCardProperty::Region] +
                addrValues[vCardProperty::Locality] +
                addrValues[vCardProperty::Street] +
                addrValues[vCardProperty::ExtendedAddress] +
                addrValues[vCardProperty::PostOfficeBox] +
                addrValues[vCardProperty::PostalCode];
    }
}

static QString getNameString(const QStringList &nameValues) {
    if (nameValues.size() != vCardProperty::Suffix + 1) {
        return nameValues.join("");
    }
    else {
        return nameValues[vCardProperty::Prefix] +
                nameValues[vCardProperty::Firstname] +
                nameValues[vCardProperty::Additional] +
                nameValues[vCardProperty::Lastname] +
                nameValues[vCardProperty::Suffix];
    }
}

void CContactDB::convertVCardsToContacts(const vCardList &vcards, QList<CContact> &contacts)
{
    for (int i = 0; i < vcards.size(); ++i) {
        CContact contact;
        if (vcards[i].isValid()) {
            vCardPropertyList properties = vcards[i].properties();
            DEBUG() << "vcards[" << i << "].properties().size: " << vcards[i].properties().size();
            for (int j = 0;  j < properties.size(); ++j) {
                vCardProperty property = properties[j];
                if (property.isValid()) {
                    if (property.name() == VC_ADDRESS) {
                        contact.setAddress(qMakePair(getParamValueByType(property.params(), vCardParam::Type),
                                                     getAddressString(property.values())));
                    }
                    /*
                    if (property.name() == VC_NAME) {
                        contact.setLastName(property.values().at(vCardProperty::Lastname));
                        contact.setFirstName(property.values().at(vCardProperty::Firstname));
                    }
                    */
                    else if (property.name() == VC_BIRTHDAY) {
                        contact.setBirthday(QDate::fromString(property.value()));
                    }
                    else if (property.name() == VC_EMAIL) {
                        contact.setEmailAddress(qMakePair(getParamValueByType(
                                   property.params(), vCardParam::Type), property.value()));
                    }
                    else if (property.name() == VC_FORMATTED_NAME) {
                        contact.setFirstName(property.value());
                    }
                    else if (property.name() == VC_NOTE) {
                        contact.setNote(property.value());
                    }
                    else if (property.name() == VC_ORGANIZATION) {
                        contact.setOrganization(property.value());
                    }
                    else if (property.name() == VC_PHOTO) {
                        QString fileName;
                        QString suffix;
                        for (int k = 0; k < property.params().size(); ++k) {
                            if (property.params().at(i).group() == vCardParam::Type) {
                                suffix = property.params().at(k).value();
                                break;
                            }
                        }
#ifdef LOCAL_DEBUG
    struct passwd *pwd = getpwuid(getuid());
    QString contactDBDir = pwd->pw_dir;
#else
    QString contactDBDir = CONTACT_DATABASE_DIR;
#endif
                        fileName = contactDBDir + QString("/") + QString::number(QDateTime::currentDateTime().toTime_t()) + QString(".") + suffix.toLower();
                        QFile file(fileName);
                        if (!file.open(QIODevice::WriteOnly)) {
                            qDebug() << "error to open";
                        }
                        file.write(QByteArray::fromBase64(property.value().toLatin1()));
                        file.close();
                        contact.setAvatar(fileName);
                    }
                    else if (property.name() == VC_TELEPHONE) {
                        contact.setPhoneNumber(qMakePair(getParamValueByType(property.params(), vCardParam::Type),
                                                         property.value()));
                    }
                    else if (property.name() == VC_URL) {
                        contact.setUrl(qMakePair(getParamValueByType(property.params(), vCardParam::Type),
                                                 property.value()));
                    }
                    else if (property.name() == VC_CUSTOM_ADDRESS) {
                        contact.setAddress(qMakePair(getParamValueByType(property.params(), vCardParam::Type),
                                                     getAddressString(property.values())));
                    }
                    else if (property.name() == VC_CUSTOM_EMAIL) {
                        contact.setEmailAddress(qMakePair(getParamValueByType(
                                   property.params(), vCardParam::Type), property.values().join("")));
                    }
                    else if (property.name() == VC_CUSTOM_URL) {
                        contact.setUrl(qMakePair(getParamValueByType(property.params(), vCardParam::Type),
                                                 property.value()));
                    }
                    else if (property.name() == VC_CUSTOM_TEL) {
                        contact.setPhoneNumber(qMakePair(getParamValueByType(property.params(), vCardParam::Type),
                                                         property.value()));
                    }
                    else if (property.name() == VC_CUSTOM_FAVORITE) {
                        contact.setIsFavorite(property.values().at(0) == "true");
                    }
                    else if (property.name() == VC_CUSTOM_GENDER) {
                        contact.setGender(property.value());
                    }
                    else if (property.name() == VC_SOUND) {
                        contact.setRingtone(property.value());
                    }
                    else if (property.name() == VC_CUSTOM_GROUP)
                    {
                        contact.setGroupId(property.value().toInt());
                    }
                    /*
                    else if (property.name() == VC_AGENT) {
                    }
                    else if (property.name() == VC_CATEGORIES) {
                    }
                    else if (property.name() == VC_CLASS) {
                    }
                    else if (property.name() == VC_DELIVERY_LABEL) {
                    }
                    else if (property.name() == VC_GEOGRAPHIC_POSITION) {
                    }
                    else if (property.name() == VC_KEY) {
                    }
                    else if (property.name() == VC_LOGO) {
                    }
                    else if (property.name() == VC_MAILER) {
                    }
                    else if (property.name() == VC_NICKNAME) {
                    }
                    else if (property.name() == VC_PRODUCT_IDENTIFIER) {
                    }
                    else if (property.name() == VC_REVISION) {
                    }
                    else if (property.name() == VC_ROLE) {
                    }
                    else if (property.name() == VC_SORT_STRING) {
                    }
                    else if (property.name() == VC_TIME_ZONE) {
                    }
                    else if (property.name() == VC_TITLE) {
                    }
                    */
                }
            }
            contacts.append(contact);
        }
    }
}

QList<CContact> CContactDB::readContactsFromVCard(const QString &path)
{
    vCardList vcards = vCard::fromFile(path);
    QList<CContact> contacts;
    convertVCardsToContacts(vcards, contacts);
    return contacts;
}

QList<CContact> CContactDB::readContactsFromString(const QString &datas)
{
    vCardList vcards = vCard::fromByteArray(datas.toLatin1());
    QList<CContact> contacts;
    convertVCardsToContacts(vcards, contacts);
    return contacts;
}

bool CContactDB::importContactsFromVCard(const QString &path)
{
    QList<CContact> contacts = readContactsFromVCard(path);
    return saveContacts(contacts);
}

static void saveContactsToVcards(vCardList &vcards, const QList<CContact> &contacts) {
    for (int i = 0; i < contacts.size(); ++i) {
        vCard vcard;
        vcard.addProperty(vCardProperty::createName(contacts[i].firstName(), ""));
        vcard.addProperty(vCardProperty(VC_FORMATTED_NAME, contacts[i].firstName(), vCardParamList()));
        // Organization
        // Gender
        // Avatar
        // Ringtone
        // Note
        // Birthday
        // IsFavorite
        // GroupId
        vcard.addProperty(vCardProperty(VC_ORGANIZATION, contacts[i].organization(), vCardParamList()));
        vcard.addProperty(vCardProperty(VC_CUSTOM_GENDER, contacts[i].gender(), vCardParamList()));

        if (!contacts[i].avatar().isEmpty()) {
            QFile file(contacts[i].avatar());
            if (file.open(QIODevice::ReadOnly) && file.isReadable()) {
                QString data = QString::fromLatin1(file.readAll().toBase64().data());
                file.close();
                QString crlfSpace(QStringLiteral("\n "));
                vCardParamList paramList;
                paramList.append(vCardParam("b", vCardParam::Encoding));
                QFileInfo info(contacts[i].avatar());
                paramList.append(vCardParam(info.suffix().toUpper(), vCardParam::Type));
                int j = MAX_LINE_LENGTH - 22 - info.suffix().length();
                for (; j < data.size(); j += MAX_LINE_LENGTH + 1) {
                    data.insert(j, crlfSpace);
                }
                vcard.addProperty(vCardProperty(VC_PHOTO, data, paramList));
            }
        }

        //vcard.addProperty(vCardProperty(VC_SOUND, ));
        vcard.addProperty(vCardProperty(VC_NOTE, contacts[i].note(), vCardParamList()));
        vcard.addProperty(vCardProperty::createBirthday(contacts[i].birthday(), vCardParamList()));
        vcard.addProperty(vCardProperty(VC_CUSTOM_FAVORITE, contacts[i].isFavorite() ? "true;1" : "false;0", vCardParamList()));
        vcard.addProperty(vCardProperty(VC_CUSTOM_GROUP, QString::number(contacts[i].groupId()), vCardParamList()));


        for (int j = 0; j < contacts[i].addresses().size(); ++j) {
            vCardParam param;
            if (!contacts[i].addresses().at(j).first.isEmpty()) {
                param.setGroup(vCardParam::Type);
                param.setValue(contacts[i].addresses().at(j).first);
            }
            vcard.addProperty(vCardProperty::createAddress(contacts[i].addresses().at(j).second,
                                                           "", "", "", "", "", "", vCardParamList() << param));
        }
        for (int j = 0; j < contacts[i].phoneNumbers().size(); ++j) {
            vCardParam param;
            if (!contacts[i].phoneNumbers().at(j).first.isEmpty()) {
                param.setGroup(vCardParam::Type);
                param.setValue(contacts[i].phoneNumbers().at(j).first);
            }
            vcard.addProperty(vCardProperty(VC_TELEPHONE, contacts[i].phoneNumbers().at(j).second, vCardParamList() << param));
        }
        for (int j = 0; j < contacts[i].emailAddresses().size(); ++j) {
            vCardParam param;
            if (!contacts[i].emailAddresses().at(j).first.isEmpty()) {
                param.setGroup(vCardParam::Type);
                param.setValue(contacts[i].emailAddresses().at(j).first);
            }
            vcard.addProperty(vCardProperty(VC_EMAIL, contacts[i].emailAddresses().at(j).second, vCardParamList() << param));
        }
        for (int j = 0; j < contacts[i].urls().size(); ++j) {
            vCardParam param;
            if (!contacts[i].urls().at(j).first.isEmpty()) {
                param.setGroup(vCardParam::Type);
                param.setValue(contacts[i].urls().at(j).first);
            }
            vcard.addProperty(vCardProperty(VC_URL, contacts[i].urls().at(j).second, vCardParamList() << param));
        }

        vcards.push_back(vcard);
    }
}

bool CContactDB::exportContactsToVCard(const QString &path)
{
    vCardList vcards;
    QList<CContact> contacts = getContacts();
    saveContactsToVcards(vcards, contacts);
    vCard::saveToFile(path, vcards);
    return true;
}

QList<CContact> CContactDB::getSimpleContacts()
{
    QList<CContact> contacts;

    QSqlQuery query(m_database);
    QString cmd = "SELECT contactId, displayLabel, avatar, pingyinName, isFavorite  FROM Contacts";
    if(!query.exec(cmd))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        DEBUG()<<Q_FUNC_INFO<<cmd;
        return contacts;
    }
    while(query.next())
    {
        CContact contact;
        contact.setId(query.value(0).toUInt());
        contact.setDisplayLabel(query.value(1).toString());
        contact.setAvatar(query.value(2).toString());
        contact.setPingyinName(query.value(3).toString());
        contact.setIsFavorite(query.value(4).toBool());
        contacts.append(contact);
    }

    return contacts;
}

CContact CContactDB::getContactById(uint id)
{
    return queryContact(id);
}

/*
static QStringList fromUints(const QList<uint> &ids) {
    QStringList strIds;

    for (int i = 0; i < ids.size(); ++i) {
        strIds.push_back(QString::number(ids[i]));
    }

    return strIds;
}
*/


QList<CContact> CContactDB::getContactsByIds(const QList<uint> &ids)
{
    QList<CContact> contacts;
    if (ids.isEmpty())
        return contacts;
    for (int i = 0; i < ids.size(); ++i) {
        contacts.push_back(queryContact(ids[i]));
    }

    return contacts;
}

QList<uint> CContactDB::getContactIdsByType(CContactDB::FilterType type)
{
    QList<uint> ids;
    QSqlQuery query(m_database);
    QString cmd;
    if (type == Birthday) {
        cmd = "SELECT contactId FROM Contacts WHERE birthday IS NOT NULL";
    }
    else {
        cmd = "SELECT contactId FROM Contacts WHERE hasEmailAddresses=1";
    }

    if(!query.exec(cmd))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        DEBUG()<<Q_FUNC_INFO<<cmd;
    }
    else
    {
        while(query.next())
        {
            ids.push_back(query.value(0).toInt());
        }
    }

    return ids;

}

QList<uint> CContactDB::getContactIdsByPhoneNumber(QString phonenumber)
{
    QList<uint> ids;
    QSqlQuery query(m_database);
    QString cmd = "SELECT contactId FROM PhoneNumbers WHERE phoneNumber=%1";

    if(!query.exec(cmd.arg(phonenumber)))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        DEBUG()<<Q_FUNC_INFO<<cmd;
    }
    else
    {
        while(query.next())
        {
            ids.push_back(query.value(0).toInt());
        }
    }

    return ids;
}

bool CContactDB::deleteMdmContacts()
{
    QList<uint> ids;
    QSqlQuery query(m_database);
    QString cmd("SELECT contactId FROM Contacts WHERE mdmId!=0");

    if(!query.exec(cmd))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        DEBUG()<<Q_FUNC_INFO<<cmd;
    }
    else
    {
        while(query.next())
        {
            ids.push_back(query.value(0).toInt());
        }
    }
    return deleteContacts(ids);
}

bool CContactDB::savePhoneNumbers(CContact contact)
{
    QSqlQuery query(m_database);

    QList<QPair<QString, QString> > phoneNumbers = contact.phoneNumbers();
    QList<QPair<QString, QString> >::const_iterator it = phoneNumbers.begin();    
    bool error = false;
    for(; it != phoneNumbers.end(); it++)
    {
        QString cmd = QString("INSERT INTO PhoneNumbers(contactId, phoneNumber, type)"
                              " VALUES('%1','%2','%3')").arg(contact.id()).arg((*it).second).arg((*it).first);
        if(!query.exec(cmd))
        {
            DEBUG()<<Q_FUNC_INFO<<query.lastError();
            DEBUG()<<Q_FUNC_INFO<<cmd;
            error = true;
        }
    }
    return !error;
}

bool CContactDB::saveEmailAddresses(CContact contact)
{
    QSqlQuery query(m_database);

    QList<QPair<QString, QString> > emailAddresses = contact.emailAddresses();
    QList<QPair<QString, QString> >::const_iterator it = emailAddresses.begin();
    bool error = false;
    for(; it != emailAddresses.end(); it++)
    {

        QString cmd = QString("INSERT INTO EmailAddresses(contactId, emailAddress, type)"
                              " VALUES('%1','%2','%3')").arg(contact.id()).arg((*it).second)
                .arg((*it).first);
        if(!query.exec(cmd))
        {
            DEBUG()<<Q_FUNC_INFO<<query.lastError();
            DEBUG()<<Q_FUNC_INFO<<cmd;
            error = true;
        }
    }
    return !error;
}

bool CContactDB::saveUrls(CContact contact)
{
    QSqlQuery query(m_database);

    QList<QPair<QString, QString> > urls = contact.urls();
    QList<QPair<QString, QString> >::const_iterator it = urls.begin();
    bool error = false;
    for(; it != urls.end(); it++)
    {
        QString cmd = QString("INSERT INTO Urls(contactId, url, type)"
                              " VALUES('%1','%2','%3')").arg(contact.id()).arg((*it).second)
                .arg((*it).first);
        if(!query.exec(cmd))
        {
            DEBUG()<<Q_FUNC_INFO<<query.lastError();
            DEBUG()<<Q_FUNC_INFO<<cmd;
            error = true;
        }
    }
    return !error;
}

bool CContactDB::saveAddresses(CContact contact)
{
    QSqlQuery query(m_database);

    QList<QPair<QString, QString> > addresses = contact.addresses();
    QList<QPair<QString, QString> >::const_iterator it = addresses.begin();
    bool error = false;
    for(; it != addresses.end(); it++)
    {
        QString cmd = QString("INSERT INTO Addresses(contactId, street, type)"
                              " VALUES('%1','%2','%3')").arg(contact.id()).arg((*it).second)
                .arg((*it).first);
        if(!query.exec(cmd))
        {
            DEBUG()<<Q_FUNC_INFO<<query.lastError();
            DEBUG()<<Q_FUNC_INFO<<cmd;
            error = true;
        }
    }
    return !error;
}

CContactDB::CContactDB()
{
#ifdef LOCAL_DEBUG
    struct passwd *pwd = getpwuid(getuid());
    QString contactDBDir = pwd->pw_dir;
#else
    QString contactDBDir = CONTACT_DATABASE_DIR;
#endif
    DEBUG() << "Database dir: " << contactDBDir;
    QString path = contactDBDir + "/" + QString(CONTACT_DATABASE_NAME);
    const bool exists = QFile::exists(path);
    m_database = UtilDB::createDatabase(contactDBDir, CONTACT_DATABASE_NAME);

    if (!m_database.open())
    {
        qWarning() << QString("Failed to open %1 database").arg(m_database.databaseName());
        qWarning() << m_database.lastError();
    }
    else
    {
        qWarning() <<QString( "Opened %1 database:").arg(m_database.databaseName());
        m_database.exec("CREATE INDEX abc ON Contacts(contactId);");
    }
    if(!exists)
    {
        QStringList schemas;
        schemas<<QString(contacts_schema[0])<<QString(contacts_schema[1])<<QString(phonenumbers_schema[1])<<QString(urls_schema[1])
                <<QString(emailAddresses_schema[1])<<QString(addresses_schema[1])<<QString(groups_schema[1]);
        UtilDB::createTable(m_database, schemas);
        DEBUG()<<"CREATE INDEX";
        m_database.exec("CREATE INDEX pnIndex ON PhoneNumbers(contactId);");
        m_database.exec("CREATE INDEX urlIndex ON Urls(contactId);");
        m_database.exec("CREATE INDEX eaIndex ON EmailAddresses(contactId);");
        m_database.exec("CREATE INDEX addIndex ON Addresses(contactId);");
    }
    initPinyinMap();
}

void CContactDB::readPhoneNumbers(CContact &contact)
{
    QSqlQuery query(m_database);
    QString cmd = QString("SELECT * FROM PhoneNumbers WHERE contactId = %1").arg(contact.id());
    if(!query.exec(cmd))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        DEBUG()<<Q_FUNC_INFO<<cmd;
    }
    else
    {
        QList<QPair<QString,QString> > pairs;
        while(query.next())
        {
            QSqlRecord record = query.record();
            QPair<QString, QString> pair;
            QString type;
            QString value;            
            for(int i = 0; i<record.count(); i++)
            {
                QSqlField field = record.field(i);
                if(field.name() == "phoneNumber")
                {
                    value = field.value().toString();
                }
                else if (field.name() == "type")
                {
                    type = field.value().toString();
                }
            }
            if(!type.isEmpty() && !value.isEmpty())
            {
                pair.first = type;
                pair.second = value;
            }
            pairs.append(pair);
        }
        contact.setPhoneNumbers(pairs);
    }
}

void CContactDB::readEmailAddresses(CContact &contact)
{

    QSqlQuery query(m_database);
    QString cmd = QString("SELECT * FROM EmailAddresses WHERE contactId = %1").arg(contact.id());
    if(!query.exec(cmd))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        DEBUG()<<Q_FUNC_INFO<<cmd;
    }
    else
    {
        QList<QPair<QString,QString> > pairs;
        while(query.next())
        {
            QSqlRecord record = query.record();
            QPair<QString, QString> pair;
            QString type;
            QString value;
            for(int i = 0; i<record.count(); i++)
            {
                QSqlField field = record.field(i);
                if(field.name() == "emailAddress")
                {
                    value = field.value().toString();
                }
                else if (field.name() == "type")
                {
                    type = field.value().toString();
                }
            }
            if(!type.isEmpty() && !value.isEmpty())
            {
                pair.first = type;
                pair.second = value;
            }
            pairs.append(pair);
        }
       contact.setEmailAddresses(pairs);
    }
}

void CContactDB::readUrls(CContact &contact)
{
    QSqlQuery query(m_database);
    QString cmd = QString("SELECT * FROM Urls WHERE contactId = %1").arg(contact.id());
    if(!query.exec(cmd))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        DEBUG()<<Q_FUNC_INFO<<cmd;
    }
    else
    {
        QList<QPair<QString,QString> > pairs;
        while(query.next())
        {
            QSqlRecord record = query.record();
            QPair<QString, QString> pair;
            QString type;
            QString value;
            for(int i = 0; i<record.count(); i++)
            {
                QSqlField field = record.field(i);
                if(field.name() == "url")
                {
                    value = field.value().toString();
                }
                else if (field.name() == "type")
                {
                    type = field.value().toString();
                }
            }
            if(!type.isEmpty() && !value.isEmpty())
            {
                pair.first = type;
                pair.second = value;
            }
            pairs.append(pair);
        }
        contact.setUrls(pairs);
    }
}

bool CContactDB::saveGroups(CContact contact)
{
    Q_UNUSED(contact)
    return true;
}

bool CContactDB::readGroups(CContact &contact)
{
    Q_UNUSED(contact)
    return true;
}

void CContactDB::readAddresses(CContact &contact)
{

    QSqlQuery query(m_database);
    QString cmd = QString("SELECT * FROM Addresses WHERE contactId = %1").arg(contact.id());
    if(!query.exec(cmd))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        DEBUG()<<Q_FUNC_INFO<<cmd;
    }
    else
    {
        QList<QPair<QString,QString> > pairs;
        while(query.next())
        {
            QSqlRecord record = query.record();
            QPair<QString, QString> pair;
            QString type;
            QString value;
            for(int i = 0; i<record.count(); i++)
            {
                QSqlField field = record.field(i);
                if(field.name() == "street")
                {
                    value = field.value().toString();
                }
                else if (field.name() == "type")
                {
                    type = field.value().toString();
                }
            }
            if(!type.isEmpty() && !value.isEmpty())
            {
                pair.first = type;
                pair.second = value;
            }
            pairs.append(pair);
        }
        contact.setAddresses(pairs);
    }
}

bool CContactDB::updateContactRecord(CContact &contact)
{
    if(contact.id() == 0)
    {
        DEBUG()<<"the contact has not exists";
        return false;
    }
    QSqlQuery query(m_database);
    saveNameDetails(contact, contact.firstName());
    QString schema = QString("update Contacts set displayLabel = '%1', firstName = '%2', lowerFirstName ='%3',"
                          "lastName ='%4', lowerLastName = '%5', middleName ='%6' , organization ='%7' , gender ='%8' , avatar ='%9' ,"
                          "ringtone ='%10' , syncTarget ='%11' , note ='%12' , pingyinName ='%13' , lowerPingyinName ='%14' , t9Index ='%15' ,"
                          "birthday ='%16' , created ='%17' , modified ='%18' , mdmId ='%19' , calendarId ='%20' , isFavorite ='%21' ,"
                          "hasPhoneNumber ='%22' , hasEmailAddresses ='%23' ,hasUrls ='%24' , hasAddresses ='%25' "
                          "  where contactId ='%26' ").arg(contact.displayLabel()).arg(contact.firstName())
            .arg(contact.lowerFirstName()).arg(contact.lastName()).arg(contact.lowerLastName())
            .arg(contact.middleName()).arg(contact.organization()).arg(contact.gender())
            .arg(contact.avatar()).arg(contact.ringtone()).arg(contact.syncTarget()).arg(contact.note())
            .arg(contact.pingyinName()).arg(contact.lowerPingyinName()).arg(contact.t9Index())
            .arg(contact.birthday().toString("yyyy-MM-dd")).arg(contact.created().toString("yyyy-MM-dd"))
            .arg(contact.modified().toString("yyyy-MM-dd")).arg(contact.mdmId())
            .arg(contact.calendarId()).arg(contact.isFavorite()).arg(contact.hasPhoneNumber())
            .arg(contact.hasEmailAddresses()).arg(contact.hasUrls()).arg(contact.hasAddresses()).arg(contact.id());

    if(!query.exec(schema))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        return false;
    }
    if(!updatePhoneNumbersRecord(contact))
        return false;
    if(!updateEmailAddressesRecord(contact))
        return false;
    if(!updateAddressesRecord(contact))
        return false;
    if(!updateUrlsRecord(contact))
        return false;

    return true;
}

bool CContactDB::updatePhoneNumbersRecord(CContact &contact)
{
    QSqlQuery query(m_database);

    QList<QPair<QString, QString> > phoneNumbers = contact.phoneNumbers();
    QList<QPair<QString, QString> >::const_iterator it = phoneNumbers.begin();
    bool error = false;
    for(; it != phoneNumbers.end(); it++)
    {
        QString cmd = QString("update PhoneNumbers set phoneNumber = '%1', type = '%2' where contactId = '%3'")
                .arg((*it).second).arg((*it).first).arg(contact.id());

        if(!query.exec(cmd))
        {
            DEBUG()<<Q_FUNC_INFO<<query.lastError();
            DEBUG()<<Q_FUNC_INFO<<cmd;
            error = true;
        }
    }
    return !error;
}

bool CContactDB::updateEmailAddressesRecord(CContact &contact)
{
    QSqlQuery query(m_database);

    QList<QPair<QString, QString> > emailAddresses = contact.emailAddresses();
    QList<QPair<QString, QString> >::const_iterator it = emailAddresses.begin();
    bool error = false;
    for(; it != emailAddresses.end(); it++)
    {
        QString cmd = QString("update EmailAddresses set emailAddress  = '%1', type = '%2' where contactId = '%3'")
                .arg((*it).second).arg((*it).first).arg(contact.id());

        if(!query.exec(cmd))
        {
            DEBUG()<<Q_FUNC_INFO<<query.lastError();
            DEBUG()<<Q_FUNC_INFO<<cmd;
            error = true;
        }
    }
    return !error;
}

bool CContactDB::updateUrlsRecord(CContact &contact)
{
    QSqlQuery query(m_database);

    QList<QPair<QString, QString> > urls = contact.urls();
    QList<QPair<QString, QString> >::const_iterator it = urls.begin();
    bool error = false;
    for(; it != urls.end(); it++)
    {
        QString cmd = QString("update Urls set url ='%1' , type ='%2'  where contactId ='%3' ")
                .arg((*it).second).arg((*it).first).arg(contact.id());

        if(!query.exec(cmd))
        {
            DEBUG()<<Q_FUNC_INFO<<query.lastError();
            DEBUG()<<Q_FUNC_INFO<<cmd;
            error = true;
        }
    }
    return !error;
}

bool CContactDB::updateAddressesRecord(CContact &contact)
{
    QSqlQuery query(m_database);

     QList<QPair<QString, QString> > addresses = contact.addresses();
     QList<QPair<QString, QString> >::const_iterator it = addresses.begin();
     bool error = false;
     for(; it != addresses.end(); it++)
     {
         QString cmd = QString("update Addresses set street = '%1', type = '%2' where contactId ='%3' ")
                 .arg((*it).second).arg((*it).first).arg(contact.id());

         if(!query.exec(cmd))
         {
             DEBUG()<<Q_FUNC_INFO<<query.lastError();
             DEBUG()<<Q_FUNC_INFO<<cmd;
             error = true;
         }
     }
     return !error;
}

bool CContactDB::updateGroupRecord(CContact &contact)
{
    Q_UNUSED(contact)
    return true;

}

CContact CContactDB::queryContact(uint id)
{
    QSqlQuery query(m_database);
    QString cmd =QString(sqlcmd[0]).arg(id).arg(id).arg(id).arg(id).arg(id);
    CContact contact;
    if (!query.exec(cmd))
    {
        DEBUG()<<Q_FUNC_INFO<<query.lastError();
        DEBUG()<<Q_FUNC_INFO<<cmd;
        return contact;
    }
    while (query.next())
    {
        readContactData(query, contact);
    }
    return contact;
}

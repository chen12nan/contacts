#ifndef CCONTACTDB_H
#define CCONTACTDB_H

#include <QtSql>
#include <db_global.h>
#include <ccontact.h>
#include "vcard.h"

class DB_EXPORT CContactDB
{
public:
    enum FilterType {
        Birthday, Email
    };

    QList<CContact> getContacts();
    bool deleteContact(uint id);
    bool saveContact(CContact contact);
    bool deleteContacts(const QList<uint> &ids);
    bool saveContacts(QList<CContact> &contacts);
    bool updateContact(CContact &contact);
    bool updateContacts(QList<CContact> &contacts);

    CContact queryContact(uint id);
    static CContactDB *instance();

    QList<CContact> readContactsFromVCard(const QString &path);
    QList<CContact> readContactsFromString(const QString &datas);
    bool importContactsFromVCard(const QString& path);
    bool exportContactsToVCard(const QString& path);
    QList<CContact> getSimpleContacts();
    CContact getContactById(uint id);
    QList<CContact> getContactsByIds(const QList<uint> &ids);
    QList<uint> getContactIdsByType(FilterType type);
    QList<uint> getContactIdsByPhoneNumber(QString phonenumber);
    bool deleteMdmContacts();


protected:
    bool innerDeleteContact(uint id);
    bool innerSaveContact(CContact &contact);
    bool savePhoneNumbers(CContact contact);
    void readPhoneNumbers(CContact &contact);
    bool saveEmailAddresses(CContact contact);
    void readEmailAddresses(CContact &contact);
    bool saveUrls(CContact contact);
    void readUrls(CContact &contact);
    bool saveGroups(CContact contact);
    bool readGroups(CContact &contact);
    bool saveAddresses(CContact contact);
    void readAddresses(CContact &contact);
    bool updateContactRecord(CContact &contact);
    bool updatePhoneNumbersRecord(CContact &contact);
    bool updateEmailAddressesRecord(CContact &contact);
    bool updateUrlsRecord(CContact &contact);
    bool updateAddressesRecord(CContact &contact);
    bool updateGroupRecord(CContact &contact);
    void saveNameDetails(CContact &contact, const QString &name);
    void convertVCardsToContacts(const vCardList &vcards, QList<CContact> &contacts);
    void readContactData(QSqlQuery &query, CContact &contact);

private:
    class ARC
    {
    public:
        ~ARC()
        {
            if(CContactDB::_instance)
            {
                delete CContactDB::_instance;
                CContactDB::_instance = NULL;
            }
        }
    };

private:
    explicit CContactDB();
    QSqlDatabase m_database;
    static CContactDB* _instance;
    static ARC arc;
};
#endif // CCONTACTDB_H

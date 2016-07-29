#include <QCoreApplication>
#include <ccontact.h>
#include <ccontactdb.h>
#include <QTime>
#include <qdebug.h>
//#define EXPORT_CONTACTS

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

#ifdef EXPORT_CONTACTS
    CContact contact1;
    contact1.init();

    CContactDB::instance()->saveContact(contact1);
    CContactDB::instance()->exportContactsToVCard("/home/zmm/savedContact.vcf");
#else
    CContactDB::instance()->importContactsFromVCard("/home/zmm/savedContact.vcf");
    QList<CContact> contacts = CContactDB::instance()->getContacts();
    DEBUG() << "_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=";
    DEBUG() << contacts.size();
    for (int i = 0; i < contacts.size(); ++i) {
        DEBUG() << contacts[i].toString();
    }
    QList<CContact> simpleContacts = CContactDB::instance()->getSimpleContacts();
    for (int i = 0; i < simpleContacts.size(); ++i) {
        DEBUG() << simpleContacts[i].toString();
    }
    QList<uint> birthdayIds = CContactDB::instance()->getContactIdsByType(CContactDB::Birthday);
    for (int i = 0; i < birthdayIds.size(); ++i) {
        DEBUG() << birthdayIds[i];
    }
    QList<uint> emailIds = CContactDB::instance()->getContactIdsByType(CContactDB::Email);
    for (int i = 0; i < emailIds.size(); ++i) {
        DEBUG() << emailIds[i];
    }
    QList<uint> phonenumberIds = CContactDB::instance()->getContactIdsByPhoneNumber("2");
    for (int i = 0; i < phonenumberIds.size(); ++i) {
        DEBUG() << phonenumberIds[i];
    }
    QList<CContact> queryContacts = CContactDB::instance()->getContactsByIds(birthdayIds);
    for (int i = 0; i < queryContacts.size(); ++i) {
        DEBUG() << queryContacts[i].toString();
    }

#endif

    return 0;//app.exec();
}


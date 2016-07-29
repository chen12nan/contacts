#include "ccontact.h"
#include <QSharedData>
#include <QDebug>

class CContactData : public QSharedData {
public:
    CContactData(): id(0), group_id(0), mdmId(0), calendarId(0), isFavorite(false)
    {

    }
    uint id;
    uint group_id;
    QString displayLabel;
    QString firstName;
    QString lowerFirstName;
    QString lastName;
    QString lowerLastName;
    QString middleName;
    QString organization;
    QString gender;
    QString avatar;
    QString ringtone;
    QString syncTarget;
    QString note;
    QString pingyinName;
    QString lowerPingyinName;
    QString t9Index;

    QDate birthday;
    QDateTime created;
    QDateTime modified;

    uint mdmId;
    uint calendarId;

    bool isFavorite;

    QList<QPair<QString, QString> > phoneNumbers;
    QList<QPair<QString, QString> > emailAddresses;
    QList<QPair<QString, QString> > addresses;
    QList<QPair<QString, QString> > urls;
};

CContact::CContact() : data(new CContactData)
{
    data->id = 0;
}

CContact::CContact(const CContact &rhs) : data(rhs.data)
{
}

CContact &CContact::operator=(const CContact &rhs)
{
    if (this != &rhs)
        data.operator=(rhs.data);
    return *this;
}

CContact::~CContact()
{
}

void CContact::init()
{
    int q = qrand();
    data->id = 0;
    data->displayLabel = QString("%1").arg(q)  + "xingkd";
    data->firstName = QString("%1").arg(q)  +  "xing";
    data->lowerFirstName =  QString("%1").arg(q)  + "xing";
    data->lastName = QString("%1").arg(q)  +  "dao";
    data->lowerLastName = QString("%1").arg(q)  +  "dao";
    data->middleName = QString("%1").arg(q)  +  "kong";
    data->organization = QString("%1").arg(q)  +  "yuanxin";
    data->gender = QString("%1").arg(q)  +  "male";
    data->avatar = "/home/zmm/testsave.jpeg";
    data->ringtone = "file::///home/xkd/hello.mp3";
    data->syncTarget = "ok";
    data->note = "Hello World";
    data->pingyinName = "xingkongdao";
    data->lowerPingyinName = "xingkongdao";
    data->t9Index = "94645664326,953";

    data->birthday = QDate(1998,7,20);
    data->created = QDateTime();
    data->modified = QDateTime();

    data->mdmId = 0;
    data->calendarId = 0;
    data->isFavorite = false;

    QPair<QString,QString> phone_0;
    phone_0.first = "HOME";
    phone_0.second = "1";
    data->phoneNumbers.append(phone_0);
    QPair<QString, QString> phone_1;
    phone_1.first = "GERNAL";
    phone_1.second = "2";
    data->phoneNumbers.append(phone_1);

    QPair<QString,QString> email_0;
    email_0.first = "GERNAL";
    email_0.second = "1@qq.com";
    data->emailAddresses.append(email_0);
    QPair<QString,QString> email_1;
    email_1.first = "WORK";
    email_1.second = "2@sina.com";
    data->emailAddresses.append(email_1);

    QPair<QString, QString> address_0;
    address_0.first = "HOME";
    address_0.second = "rong-ze-jia-yuan";
    data->addresses.append(address_0);
    QPair<QString,QString> address_1;
    address_1.first = "WORK";
    address_1.second = "hang-xin-yuan";
    data->addresses.append(address_1);

    QPair<QString,QString> url_0;
    url_0.first = "WEBSITE";
    url_0.second = "www.baidu.com";
    data->urls.append(url_0);
}
void CContact::setId(uint id)
{
    data->id = id;
}
// data->id   调用下列函数
// T *	operator->()
uint CContact::id() const
{
    return data->id;
}

void CContact::setGroupId(uint id)
{
    data->group_id = id;
}

uint CContact::groupId() const
{
    return data->group_id;
}

void CContact::setDisplayLabel(const QString &displayLabel)
{
    data->displayLabel = displayLabel;
}

QString CContact::displayLabel() const
{
    return data->displayLabel;
}

void CContact::setFirstName(const QString &firstName)
{
    data->firstName = firstName;
}

QString CContact::firstName() const
{
    return data->firstName;
}

void CContact::setLowerFirstName(const QString &lowerFirstName)
{
    data->lowerFirstName = lowerFirstName;
}

QString CContact::lowerFirstName() const
{
    return data->lowerFirstName;
}

void CContact::setLastName(const QString &lastName)
{
    data->lastName = lastName;
}

QString CContact::lastName() const
{
    return data->lastName;
}

void CContact::setLowerLastName(const QString &lowerLastName)
{
    data->lowerLastName = lowerLastName;
}

QString CContact::lowerLastName() const
{
    return data->lowerLastName;
}

void CContact::setMiddleName(const QString &middleName)
{
    data->middleName = middleName;
}

QString CContact::middleName() const
{
    return data->middleName;
}

void CContact::setOrganization(const QString &organization)
{
    data->organization = organization;
}

QString CContact::organization() const
{
    return data->organization;
}

void CContact::setGender(const QString &gender)
{
    data->gender = gender;
}

QString CContact::gender() const
{
    return data->gender;
}

void CContact::setAvatar(const QString &avatar)
{
    data->avatar = avatar;
}

QString CContact::avatar() const
{
    return data->avatar;
}

void CContact::setRingtone(const QString &ringtone)
{
    data->ringtone = ringtone;
}

QString CContact::ringtone() const
{
    return data->ringtone;
}

void CContact::setSyncTarget(const QString &syncTarget)
{
    data->syncTarget = syncTarget;
}

QString CContact::syncTarget() const
{
    return data->syncTarget;
}

void CContact::setNote(const QString &note)
{
    data->note = note;
}

QString CContact::note() const
{
    return data->note;
}

QString CContact::pingyinName() const
{
    return data->pingyinName;
}

QString CContact::lowerPingyinName() const
{
    return data->lowerPingyinName;
}

QString CContact::t9Index() const
{
    return data->t9Index;
}

void CContact::setBirthday(const QDate &birthday)
{
    data->birthday = birthday;
}

QDate CContact::birthday() const
{
    return data->birthday;
}

QDateTime CContact::created() const
{
    return data->created;
}

QDateTime CContact::modified() const
{
    return data->modified;
}

uint CContact::mdmId() const
{
    return data->mdmId;
}

uint CContact::calendarId() const
{
    return data->calendarId;
}

void CContact::setIsFavorite(bool isFavorite)
{
    data->isFavorite = isFavorite;
}

bool CContact::isFavorite() const
{
    return data->isFavorite;
}

bool CContact::hasPhoneNumber() const
{
    return data->phoneNumbers.count() > 0;
}

bool CContact::hasEmailAddresses() const
{
    return data->emailAddresses.count() > 0;
}

bool CContact::hasUrls() const
{
    return data->urls.count() > 0;
}

bool CContact::hasAddresses() const
{
    return data->addresses.count() > 0;
}

void CContact::setPhoneNumbers(const QList<QPair<QString, QString> > &phoneNumbers)
{
    data->phoneNumbers = phoneNumbers;
}

void CContact::setPhoneNumber(const QPair<QString, QString> &phoneNumber)
{
    data->phoneNumbers.append(phoneNumber);
}

QList<QPair<QString, QString> > CContact::phoneNumbers() const
{
    return data->phoneNumbers;
}

void CContact::setEmailAddresses(const QList<QPair<QString, QString> > &emailAddresses)
{
    data->emailAddresses = emailAddresses;
}

void CContact::setEmailAddress(const QPair<QString, QString> &emailAddress)
{
    data->emailAddresses.append(emailAddress);
}

QList<QPair<QString, QString> > CContact::emailAddresses() const
{
    return data->emailAddresses;
}

void CContact::setAddresses(const QList<QPair<QString, QString> > &addresses)
{
    data->addresses = addresses;
}

void CContact::setAddress(const QPair<QString, QString> &address)
{
    data->addresses.append(address);
}

QList<QPair<QString, QString> > CContact::addresses() const
{
    return data->addresses;
}

void CContact::setUrls(const QList<QPair<QString, QString> > &urls)
{
    data->urls = urls;
}

void CContact::setUrl(const QPair<QString, QString> &url)
{
    data->urls.append(url);
}

QList<QPair<QString, QString> > CContact::urls() const
{
    return data->urls;
}

QString CContact::toString()
{
    QString phoneNumber;
    QString emailAddress;
    QString address;
    QString url;
    for (int i = 0; i < data->phoneNumbers.size(); ++i) {
        phoneNumber.append(QString("type: %1; number: %2\n").arg(data->phoneNumbers.at(i).first, data->phoneNumbers.at(i).second));
    }
    for (int i = 0; i < data->emailAddresses.size(); ++i) {
        emailAddress.append(QString("type: %1; number: %2\n").arg(data->emailAddresses.at(i).first, data->emailAddresses.at(i).second));
    }
    for (int i = 0; i < data->addresses.size(); ++i) {
        address.append(QString("type: %1; number: %2\n").arg(data->addresses.at(i).first, data->addresses.at(i).second));
    }
    for (int i = 0; i < data->urls.size(); ++i) {
        url.append(QString("type: %1; number: %2\n").arg(data->urls.at(i).first, data->urls.at(i).second));
    }

    return QString("id: %1;\ngroup_id: %2\n;displayLabel: %3;\nfirstName: %4;\nlastName: %5;\nnote: %6;\npingyinName: %7;\nbirthday: %8;\nisFavorite: %9")
            .arg(data->id).arg(data->group_id).arg(data->displayLabel).arg(data->firstName).arg(data->lastName).arg(data->note).arg(data->pingyinName).arg(data->birthday.toString()).arg(data->isFavorite ? "true" : "false")
            + QString("\nphoneNumbers: %1;\nemailAddresses: %2;\naddresses:%3;\nurls: %4\n")
            .arg(phoneNumber, emailAddress, address, url);
}

void CContact::setPingyinName(const QString &pingyinName)
{
    data->pingyinName = pingyinName;
}

void CContact::setLowerPingyinName(const QString &pingyinName)
{
    data->lowerPingyinName = pingyinName;
}

void CContact::setT9Index(const QString &t9Index)
{
    data->t9Index = t9Index;
}

void CContact::setCreated(const QDateTime &dateTime)
{
    data->created = dateTime;
}

void CContact::setModified(const QDateTime &dateTime)
{
    data->modified = dateTime;
}

void CContact::setMdmId(uint mdmId)
{
    data->mdmId = mdmId;
}

void CContact::setCalendarId(uint calenderId)
{
    data->calendarId = calenderId;
}

QDebug &operator<<(QDebug dbg, const CContact &contact)
{
    dbg.nospace()<<"CContact( id: "<<contact.id()<<" , ";
    dbg.nospace()<<"displayLabel: "<<contact.displayLabel()<<" , ";
    dbg.nospace()<<"firstName: "<<contact.firstName()<<" , ";
    dbg.nospace()<<"lowerFirstName: "<<contact.lowerFirstName()<<" , ";
    dbg.nospace()<<"lastName: "<<contact.lastName()<<" , ";
    dbg.nospace()<<"lowerLastName: "<<contact.lowerLastName()<<" , ";
    dbg.nospace()<<"middle: "<<contact.middleName()<<" , ";
    dbg.nospace()<<"organization: "<<contact.organization()<<" , ";
    dbg.nospace()<<"gender: "<<contact.gender()<<" , ";
    dbg.nospace()<<"avatar: "<<contact.avatar()<<" , ";
    dbg.nospace()<<"ringtone: "<<contact.ringtone()<<" , ";
    dbg.nospace()<<"syncTarget: "<<contact.syncTarget()<<" , ";
    dbg.nospace()<<"note: "<<contact.note()<<" , ";
    dbg.nospace()<<"pingyinName: "<<contact.pingyinName()<<" , ";
    dbg.nospace()<<"lowerPingyinName: "<<contact.lowerPingyinName()<<" , ";
    dbg.nospace()<<"t9Index: "<<contact.t9Index()<<" , ";
    dbg.nospace()<<"birthday: "<<contact.birthday()<<" , ";
    dbg.nospace()<<"created: "<<contact.created()<<" , ";
    dbg.nospace()<<"modified: "<<contact.modified()<<" , ";
    dbg.nospace()<<"mdmId: "<<contact.mdmId()<<" , ";
    dbg.nospace()<<"calendarId: "<<contact.calendarId()<<" , ";
    dbg.nospace()<<"isFavorite: "<<contact.isFavorite()<<" , ";
    dbg.nospace()<<"phoneNumbers: "<<contact.phoneNumbers()<<" , ";
    dbg.nospace()<<"emailAddresses: "<<contact.emailAddresses()<<" , ";
    dbg.nospace()<<"addresses: "<<contact.addresses()<<" , ";
    dbg.nospace()<<"urls: "<<contact.urls()<<" )";
    return dbg.maybeSpace();
}


QDataStream &operator<<(QDataStream &stream, const CContact &contact)
{
    stream<<contact.id();
    stream<<contact.displayLabel();
    stream<<contact.firstName();
    stream<<contact.lowerFirstName();
    stream<<contact.lastName();
    stream<<contact.lowerLastName();
    stream<<contact.middleName();
    stream<<contact.organization();
    stream<<contact.gender();
    stream<<contact.avatar();
    stream<<contact.ringtone();
    stream<<contact.syncTarget();
    stream<<contact.note();
    stream<<contact.pingyinName();
    stream<<contact.lowerPingyinName();
    stream<<contact.t9Index();
    stream<<contact.birthday();
    stream<<contact.created();
    stream<<contact.modified();
    stream<<contact.mdmId();
    stream<<contact.calendarId();
    stream<<contact.isFavorite();
    stream<<contact.phoneNumbers();
    stream<<contact.emailAddresses();
    stream<<contact.addresses();
    stream<<contact.urls();
    return stream;
}


QDataStream &operator>>(QDataStream &stream, CContact &contact)
{
    stream>>contact.data->id;
    stream>>contact.data->displayLabel;
    stream>>contact.data->firstName;
    stream>>contact.data->lowerFirstName;
    stream>>contact.data->lastName;
    stream>>contact.data->lowerLastName;
    stream>>contact.data->middleName;
    stream>>contact.data->organization;
    stream>>contact.data->gender;
    stream>>contact.data->avatar;
    stream>>contact.data->ringtone;
    stream>>contact.data->syncTarget;
    stream>>contact.data->note;
    stream>>contact.data->pingyinName;
    stream>>contact.data->lowerPingyinName;
    stream>>contact.data->t9Index;
    stream>>contact.data->birthday;
    stream>>contact.data->created;
    stream>>contact.data->modified;
    stream>>contact.data->mdmId;
    stream>>contact.data->calendarId;
    stream>>contact.data->isFavorite;
    stream>>contact.data->phoneNumbers;
    stream>>contact.data->emailAddresses;
    stream>>contact.data->addresses;
    stream>>contact.data->urls;
    return stream;
}

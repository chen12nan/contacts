#ifndef CCONTACT_H
#define CCONTACT_H

#include <QSharedDataPointer>
#include <QDateTime>
#include <QPair>
#include <QDataStream>
#include "ccontact_global.h"

class CContactData;
class CContactDB;

class SYBERPIM_EXPORT CContact
{
public:
    CContact();
    CContact(const CContact &);
    CContact &operator=(const CContact &);
    ~CContact();


    void init();
    void setId(uint id);
    uint id() const;

    void setGroupId(uint id);
    uint groupId() const;

    void setDisplayLabel(const QString& displayLabel);
    QString displayLabel() const;

    void setFirstName(const QString& firstName);
    QString firstName() const;

    void setLowerFirstName(const QString& lowerFirstName);
    QString lowerFirstName() const;

    void setLastName(const QString& lastName);
    QString lastName() const;

    void setLowerLastName(const QString& lowerLastName);
    QString lowerLastName() const;

    void setMiddleName(const QString& middleName);
    QString middleName() const;

    void setOrganization(const QString& organization);
    QString organization() const;

    void setGender(const QString& gender);
    QString gender() const;

    void setAvatar(const QString& avatar);
    QString avatar() const;

    void setRingtone(const QString& ringtone);
    QString ringtone() const;

    void setSyncTarget(const QString& syncTarget);
    QString syncTarget() const;

    void setNote(const QString& note);
    QString note() const;

//    void setPingyinName(const QString& pingyinName);
    QString pingyinName() const;

//    void setLowerPingyinName(const QString& pingyinName);
    QString lowerPingyinName() const;

//    void setT9Index(const QString& t9Index);
    QString t9Index() const;

    void setBirthday(const QDate& birthday);
    QDate birthday() const;

    QDateTime created() const;

    QDateTime modified() const;

    uint mdmId() const;

    uint calendarId() const;

    void setIsFavorite(bool isFavorite);
    bool isFavorite() const;

    bool hasPhoneNumber() const;
    bool hasEmailAddresses() const;
    bool hasUrls() const;
    bool hasAddresses() const;

    void setPhoneNumbers(const QList<QPair<QString, QString> >& phoneNumbers);
    void setPhoneNumber(const QPair<QString, QString>& phoneNumber);
    QList<QPair<QString, QString> > phoneNumbers() const;

    void setEmailAddresses(const QList<QPair<QString, QString> >& emailAddresses);
    void setEmailAddress(const QPair<QString, QString>& emailAddress);
    QList<QPair<QString, QString> > emailAddresses() const;

    void setAddresses(const QList<QPair<QString, QString> >& addresses);
    void setAddress(const QPair<QString, QString>& address);
    QList<QPair<QString, QString> > addresses() const;

    void setUrls(const QList<QPair<QString, QString> >& urls);
    void setUrl(const QPair<QString, QString>& url);
    QList<QPair<QString, QString> > urls() const;

    // for qlist indexof
    inline bool operator==(const CContact &other)
    { return this->data == other.data; }
    QString toString();

private:
    void setPingyinName(const QString& pingyinName);
    void setLowerPingyinName(const QString& pingyinName);
    void setT9Index(const QString& t9Index);
    void setCreated(const QDateTime& dateTime);
    void setModified(const QDateTime& dateTime);
    void setMdmId(uint mdmId);
    void setCalendarId(uint calenderId);
    friend QDebug& operator<<(QDebug dbg, const CContact& contact);
    friend QDataStream &	operator<<(QDataStream & stream, const CContact & contact);
    friend QDataStream &	operator>>(QDataStream & stream, CContact & contact);
    friend class CContactDB;
private:
    QSharedDataPointer<CContactData> data;
};

QDebug& operator<<(QDebug dbg, const CContact& contact);
QDataStream &	operator<<(QDataStream & stream, const CContact & contact);
QDataStream &	operator>>(QDataStream & stream, CContact & contact);
#endif // CCONTACT_H

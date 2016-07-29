TARGET = contact_sqlite
TEMPLATE = lib
DESTDIR = $(HOME)/.build/lib

QT += sql dbus

INCLUDEPATH += ../syber_contact

HEADERS += \
    ccontactdb.h \
    db_global.h \
    utildb.h \
    vcard.h \
    vcardparam.h \
    vcardproperty.h \
    libvcard_global.h

SOURCES += \
    ccontactdb.cpp \
    utildb.cpp \
    vcard.cpp \
    vcardparam.cpp \
    vcardproperty.cpp

DEFINES += DB_LIBRARY
DEFINES += LOCAL_DEBUG

MOC_DIR = .moc
OBJECTS_DIR = .obj
RESOURCES += \
    unicode2pinyin.qrc

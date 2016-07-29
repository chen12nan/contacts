QT += sql dbus

LIBS += -L$(HOME)/.build/lib
LIBS += -lcontact_sqlite -lsyber_contact

INCLUDEPATH += ../syber_contact\
               ../contact_sqlite
SOURCES += \
    main.cpp

TARGET = syber_contact
TEMPLATE = lib
DESTDIR = $(HOME)/.build/lib

QT += sql dbus
DEFINES += SYBERPIM_LIBRARY

HEADERS += \
    ccontact.h \
    ccontact_global.h

SOURCES += \
    ccontact.cpp

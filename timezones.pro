######################################################################
# Automatically generated by qmake (2.01a) ?? ???? 28 23:27:21 2010
######################################################################

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = timezones
#
TRANSLATIONS = timezones_ru_RU.ts
#
INCLUDEPATH = src/headers/
# Input
HEADERS += src/headers/lcdclock.h \
           src/headers/timezones.h \
           src/headers/townset.h \
    src/headers/spinboxdelegate.h
		   
FORMS += src/forms/timezones.ui \
		 src/forms/townset.ui
SOURCES += src/sources/lcdclock.cpp \
           src/sources/main.cpp \
           src/sources/timezones.cpp \
           src/sources/townset.cpp \
    src/sources/spinboxdelegate.cpp
###
### путь к инсталюции
target.path=/usr/bin/timezones
### установить
INSTALLS+=target

RESOURCES += \
    src/timezones.qrc

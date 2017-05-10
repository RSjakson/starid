TARGET = starid
TEMPLATE = lib
CONFIG += c++14 warn_off debug

DIR=/home/noah/starid
include($$DIR/stars/stars.pri)
include($$DIR/identification/identification.pri)
INCLUDEPATH += \
    $$DIR/stars \
    $$DIR/identification \
    $$DIR/lib \
    /usr/include/python3.5m \

SOURCES += libstarid.cpp
HEADERS += libstarid.h


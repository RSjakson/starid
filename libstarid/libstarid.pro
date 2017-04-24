TARGET = starid
TEMPLATE = lib
CONFIG -= qt
CONFIG += c++14 warn_off debug

SOURCES += \
    starid.cpp

HEADERS += \
    starid.h

INCLUDEPATH += /home/noah/tf/lib/python3.5/site-packages/tensorflow/include

SOURCES += \
    ../stars/globals.cpp \
    ../stars/pointing_vectors.cpp \
    ../stars/sky.cpp \
    ../stars/float_int_table.cpp \
    ../stars/star_catalog.cpp \

HEADERS += \
    ../stars/globals.h \
    ../stars/pointing_vectors.h \
    ../stars/sky.h \
    ../stars/float_int_table.h \
    ../stars/star_catalog.h \

INCLUDEPATH += ../stars

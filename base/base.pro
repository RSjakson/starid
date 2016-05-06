TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++14
CONFIG += warn_off

TARGET = base

SOURCES += \
    catalog.cpp \
    sensor.cpp \
    pointing.cpp \
    main.cpp

HEADERS += \
    catalog.h \
    sensor.h \
    pointing.h

INCLUDEPATH += ../../armadillo
LIBS += ../../armadillo/libarmadillo.so



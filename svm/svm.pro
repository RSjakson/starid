include (../base/base.pri)

TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++14
CONFIG += warn_off

TARGET = svm

SOURCES += \
    problem.cpp \
    solver.cpp \
    main.cpp

HEADERS += \
    problem.h \
    solver.h

INCLUDEPATH += ../../armadillo
LIBS += ../../armadillo/libarmadillo.so

DISTFILES += \
    svm.pri


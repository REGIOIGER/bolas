TEMPLATE = app TARGET = name_of_the_app

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

HEADERS += \
    caja.h \
    Particle.h \
    Vector3D.h

SOURCES += \
    caja.cpp \
    main.cpp \
    Particle.cpp \
    Vector3D.cpp


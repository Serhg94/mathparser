QT += core
QT -= gui

CONFIG += c++11

TARGET = MathParser
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mathparser.cpp

HEADERS += \
    mathparser.h \
    token.h

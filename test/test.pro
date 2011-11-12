TEMPLATE = app
TARGET = aerowindow

win32 {
#  LIBS +=  -ldwmapi
}
SOURCES = main.cpp
include(../src/libqaerowidget.pri)

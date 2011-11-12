include(../config.pri)
CONFIG *= staticlib

#QT += network
QAEROWIDGET_SRCPATH = $$PWD
PROJECT_LIBDIR = $$PWD/../lib$${PLATFORM_EXT}$${ARCH_EXT}$${TOOLCHAIN_EXT}

TEMPLATE += fakelib
QAEROWIDGET_NAME = $$qtLibraryTarget(QAeroWidget)
TEMPLATE -= fakelib

INCLUDEPATH += $$QAEROWIDGET_SRCPATH
DEPENDPATH += $$QAEROWIDGET_SRCPATH

!qaerowidget-buildlib {
	static*: PRE_TARGETDEPS += $$PROJECT_LIBDIR/$$qtStaticLib(QAeroWidget)
	LIBS += -L$$PROJECT_LIBDIR  -l$$QAEROWIDGET_NAME
} else {
#src
	SOURCES = \
		qaerowindow.cpp

	HEADERS = \
		qaerowindow.h

	win32 {
			SOURCES += qaerowindow_win.cpp
	} unix {
			SOURCES += qaerowindow_linux.cpp
	}

}

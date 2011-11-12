TEMPLATE = lib
CONFIG *= staticlib qaerowidget-buildlib
VERSION = 1.0.2
CONFIG += loaddll

include(libqaerowidget.pri)
DESTDIR = $$PROJECT_LIBDIR/../lib$${PLATFORM_EXT}$${ARCH_EXT}$${TOOLCHAIN_EXT}
TARGET = $$QAEROWIDGET_NAME

loaddll {
	DEFINES += LOAD_DLL
} else {

}












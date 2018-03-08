TEMPLATE = lib
CONFIG += qt warn_on
QT -= gui
unix:!macx: CONFIG += staticlib

# The ABI version.
win32: VERSION = 0.7.3.0  # major.minor.patch.build
else: VERSION  = 0.7.3    # major.minor.patch
# 1.0.0 is the first stable ABI.
# The next binary incompatible change will be 2.0.0 and so on.
# The existing QuaZIP policy on changing ABI requires to bump the
# major version of QuaZIP itself as well. Note that there may be
# other reasons for chaging the major version of QuaZIP, so
# in case where there is a QuaZIP major version bump but no ABI change,
# the VERSION variable will stay the same.

# For example:

# QuaZIP 1.0 is released after some 0.x, keeping binary compatibility.
# VERSION stays 1.0.0.
# Then some binary incompatible change is introduced. QuaZIP goes up to
# 2.0, VERSION to 2.0.0.
# And so on.

if (contains(QT_ARCH, x86_64)|contains(QT_ARCH, arm64)) {
    ARCH = 64
    STG_ARCH = x86_64
} else {
    ARCH = 32
    STG_ARCH = x86
}

win32 {

    QMAKE_EXT_OBJ = .obj
    CONFIG += windows
    greaterThan(QT_MAJOR_VERSION, 4): LIBS += -lz

    QMAKE_TARGET_COMPANY = "Sergey A. Tachenov"
    QMAKE_TARGET_DESCRIPTION = "C++ wrapper over Gilles Vollant's ZIP/UNZIP"
    QMAKE_TARGET_COPYRIGHT = "Copyright (C) 2005-2014 Sergey A. Tachenov"
    QMAKE_TARGET_PRODUCT = "QuaZIP ($$join(ARCH,,,bit))"

}

macx: LIBS += -lz

CONFIG += skip_target_version_ext
unix: !macx: TARGET = quazip
else:        TARGET = QuaZIP

# You'll need to define this one manually if using a build system other
# than qmake or using QuaZIP sources directly in your project.

# Indicate build type
staticlib {
    BUILD    = Static
    DEFINES += QUAZIP_STATIC
} else {
    # This one handles dllimport/dllexport directives.
    BUILD    = Shared
    DEFINES += QUAZIP_BUILD
}

CONFIG(debug, debug|release) {
    BUILD += Debug Build
    ARCH_BLD = bit_debug
    macx: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d07)
    unix:!macx: TARGET = $$join(TARGET,,,d)
} else {
    BUILD += Release Build
    ARCH_BLD = bit_release
    win32: TARGET = $$join(TARGET,,,07)
}
DESTDIR = $$join(ARCH,,,$$ARCH_BLD)
message("~~~ QUAZIP $$join(ARCH,,,bit) $${BUILD} ~~~")

PRECOMPILED_DIR = $$DESTDIR/.pch
OBJECTS_DIR     = $$DESTDIR/.obj
MOC_DIR         = $$DESTDIR/.moc
RCC_DIR         = $$DESTDIR/.qrc
UI_DIR          = $$DESTDIR/.ui

# Input files
include(quazip.pri)
include(../LPub3DPlatformSpecific.pri)


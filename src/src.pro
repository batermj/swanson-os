TEMPLATE = app

CONFIG += testcase
CONFIG -= qt

TARGET = swanson-test

HEADERS += "assert.h"
SOURCES += "assert.c"

HEADERS += "crc32.h"
SOURCES += "crc32.c"

HEADERS += "debug.h"
SOURCES += "debug.c"

HEADERS += "gpt.h"
SOURCES += "gpt.c"
SOURCES += "gpt-source.c"

HEADERS += "gpt-test.h"
SOURCES += "gpt-test.c"

HEADERS += "guid.h"
SOURCES += "guid.c"

HEADERS += "memmap.h"
SOURCES += "memmap.c"

HEADERS += "memmap-test.h"
SOURCES += "memmap-test.c"

HEADERS += "sstream.h"
SOURCES += "sstream.c"

HEADERS += "stream.h"
SOURCES += "stream.c"

HEADERS += "test.h"
SOURCES += "test.c"

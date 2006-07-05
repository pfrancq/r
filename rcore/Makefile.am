## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS        = foreign

SUBDIRS                 = std tools db prg app . frontend

CPPFLAGS                = $(INCLUDES_R)

#include_HEADERS         = error.h

lib_LTLIBRARIES         = librcore.la
librcore_la_SOURCES     =
librcore_la_LDFLAGS     = -no-undefined -version-info 2:0:0
librcore_la_LIBADD      = -lm -lcurl -lpthread tools/libtools.la db/libdb.la prg/libprg.la std/libstd.la app/libapp.la

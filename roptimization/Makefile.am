## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS        = foreign

SUBDIRS                 = promethee ga gga ga2d . frontend

CPPFLAGS                = $(INCLUDES_R)

#include_HEADERS         = error.h

lib_LTLIBRARIES         = libroptimization.la
libroptimization_la_SOURCES     = 
libroptimization_la_LDFLAGS     = -no-undefined -version-info 2:0:0
libroptimization_la_LIBADD = ga/libga.la gga/libgga.la ga2d/libga2d.la promethee/libpromethee.la ../rcore/librcore.la ../rmath/librmath.la

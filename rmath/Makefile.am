## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS        = foreign

SUBDIRS                 = matrix graph geometry .

#INCLUDES = -I$(top_srcdir) $(all_includes) $(INCLUDES_R)

CPPFLAGS                = $(INCLUDES_R)

include_HEADERS         = rmath.h

lib_LTLIBRARIES         = librmath.la
librmath_la_SOURCES     = rmath.cpp
librmath_la_LDFLAGS     = -no-undefined -version-info 2:0:0
librmath_la_LIBADD      = geometry/libgeometry.la graph/libgraph.la matrix/libmatrix.la	../rcore/librcore.la

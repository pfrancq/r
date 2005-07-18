INCLUDES = -I$(top_srcdir) $(all_includes) $(INCLUDES_R)

AUTOMAKE_OPTIONS        = foreign
SUBDIRS =

lib_LTLIBRARIES         = librcorekde.la

librcorekde_la_METASOURCES = AUTO

rincludedir = $(includedir)/r
rinclude_HEADERS = rqt.h
librcorekde_la_SOURCES =  rqt.cpp
librcorekde_la_LIBADD = ../../librcore.la $(LIB_KDECORE) $(LIB_KDEUI)
librcorekde_la_LDFLAGS     = -no-undefined -version-info 2:0:0

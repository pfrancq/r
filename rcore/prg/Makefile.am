INCLUDES = -I$(top_srcdir)  $(all_includes) $(INCLUDES_R)
noinst_LTLIBRARIES =  libprg.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
prgincludedir = $(includedir)/r
prginclude_HEADERS = rprgfunc.h rprginst.h rprgvarconst.h rprgvarref.h \
		rprg.h rprgclass.h rprginstfor.h rprgoutput.h \
		 rprginstmethod.h rprgvar.h rprgvarval.h

libprg_la_SOURCES = rprgfunc.cpp rprginstfor.cpp \
		rprgvarconst.cpp rprgvarref.cpp rprg.cpp rprgoutput.cpp \
		rprgclass.cpp rprginst.cpp rprginstmethod.cpp rprgvar.cpp rprgvarval.cpp

INCLUDES = -I$(top_srcdir) -I$(top_srcdir)/include/r
pkglib_LTLIBRARIES =  librprg.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
SUBDIRS =
rprgincludedir = $(includedir)/r/rprg
rprginclude_HEADERS = rprgfunc.h rprginst.h rprgvarconst.h rprgvarref.h \
		rprg.h rprgclass.h rprginstfor.h rprgoutput.h \
		 rprginstmethod.h rprgvar.h rprgvarval.h

librprg_la_SOURCES = rprgfunc.cpp rprginstfor.cpp \
		rprgvarconst.cpp rprgvarref.cpp rprg.cpp rprgoutput.cpp \
		rprgclass.cpp rprginst.cpp rprginstmethod.cpp rprgvar.cpp rprgvarval.cpp

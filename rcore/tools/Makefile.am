INCLUDES = -I$(top_srcdir) $(all_includes) $(INCLUDES_R)
noinst_LTLIBRARIES =  libtools.la
AM_CFLAGS =
AM_CXXFLAGS =
AM_FFLAGS =
stdincludedir = $(includedir)/r
stdinclude_HEADERS =  rmsg.h rstring.h rchar.h \
					random.h rio.h rtextfile.h rrecfile.h rrecfile.hh rdate.h \
					rlang.h rlocale.h \
					rxmlattr.h rxmlfile.h rxmlstruct.h rxmltag.h rhtmlfile.h \
					rdownload.h rtextencoding.h rcstring.h rfile.h \
					rdir.h riofile.h

libtools_la_SOURCES =  random.cpp rdate.cpp rio.cpp rlang.cpp rlocale.cpp rmsg.cpp \
				rstring.cpp rtextfile.cpp rchar.cpp rxmlattr.cpp \
				rxmlfile.cpp rxmlstruct.cpp rxmltag.cpp rhtmlfile.cpp \
				rdownload.cpp rtextencoding.cpp rcstring.cpp rfile.cpp rdir.cpp \
				riofile.cpp

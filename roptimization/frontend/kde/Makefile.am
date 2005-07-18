INCLUDES = -I$(top_srcdir) $(all_includes) $(INCLUDES_R)

AUTOMAKE_OPTIONS        = foreign

lib_LTLIBRARIES         = libroptimizationkde.la

libroptimizationkde_la_METASOURCES = AUTO

SUBDIRS =
rincludedir = $(includedir)/r
rinclude_HEADERS = qgamonitor.h qpromcriteria.h qxmlcontainer.h qdrawpolygons.h
libroptimizationkde_la_SOURCES =  qgamonitor.cpp qpromcriteria.cpp qxmlcontainer.cpp qdrawpolygons.cpp
libroptimizationkde_la_LDFLAGS     = -no-undefined -version-info 2:0:0
libroptimizationkde_la_LIBADD = ../../libroptimization.la ../../../rcore/frontend/kde/librcorekde.la

PROJECT(rmath)

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(../rcore/std)
INCLUDE_DIRECTORIES(../rcore/tools)
INCLUDE_DIRECTORIES(../rcore/geometry)

SET(rmath_INST_INCLUDES rmath.h)
SET(rmath_TARGET_SOURCES rmath.cpp)

ADD_SOURCE_FILE(rmath geometry RMATH_INCLUDES)
ADD_SOURCE_FILE(rmath graph RMATH_INCLUDES)
ADD_SOURCE_FILE(rmath matrix RMATH_INCLUDES)

INCLUDE_DIRECTORIES(${RMATH_INCLUDES})
ADD_LIBRARY(rmath SHARED ${rmath_TARGET_SOURCES})
TARGET_LINK_LIBRARIES(rmath rcore)

SET_TARGET_PROPERTIES(rmath PROPERTIES LT_VERSION_CURRENT 2
                                       VERSION 2.0.0
                                       SOVERSION 2)

CREATE_LIBTOOL_FILE(rmath /lib)

INSTALL(TARGETS rmath LIBRARY DESTINATION lib)

IF(rcmake-verbose)
PRINT_LIST_WITH_MESSAGE("Will install the following header files for rmath:" rmath_INST_INCLUDES)
ENDIF(rcmake-verbose)

INSTALL(FILES ${rmath_INST_INCLUDES} DESTINATION include/r)
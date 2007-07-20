PROJECT(roptimization)

INCLUDE_DIRECTORIES(.)
INCLUDE_DIRECTORIES(../rcore/std)
INCLUDE_DIRECTORIES(../rcore/tools)
INCLUDE_DIRECTORIES(../rcore/app)
INCLUDE_DIRECTORIES(../rcore/geometry)
INCLUDE_DIRECTORIES(../rcore/frontend/kde)
INCLUDE_DIRECTORIES(../rmath/geometry)
INCLUDE_DIRECTORIES(../rmath/graph)

ADD_SOURCE_FILE(roptimization ga ROPTIMIZATION_INCLUDES)
ADD_SOURCE_FILE(roptimization ga2d ROPTIMIZATION_INCLUDES)
ADD_SOURCE_FILE(roptimization gga ROPTIMIZATION_INCLUDES)
ADD_SOURCE_FILE(roptimization promethee ROPTIMIZATION_INCLUDES)

INCLUDE_DIRECTORIES(${ROPTIMIZATION_INCLUDES})
ADD_LIBRARY(roptimization SHARED ${roptimization_TARGET_SOURCES})
TARGET_LINK_LIBRARIES(roptimization rcore rmath)

SET_TARGET_PROPERTIES(roptimization PROPERTIES LT_VERSION_CURRENT 2
                                               VERSION 2.0.0
                                               SOVERSION 2)

CREATE_LIBTOOL_FILE(roptimization /lib)

INSTALL(TARGETS roptimization LIBRARY DESTINATION lib)

IF(rcmake-verbose)
PRINT_LIST_WITH_MESSAGE("Will install the following header files for roptimization:" roptimization_INST_INCLUDES)
ENDIF(rcmake-verbose)

INSTALL(FILES ${roptimization_INST_INCLUDES} DESTINATION include/r)

ADD_SOURCE_FILE(roptimization frontend ROPTIMIZATION_INCLUDES)

ADD_FRONTEND(roptimization 2 2 2.0.0 lib r ROPTIMIZATION_INCLUDES)
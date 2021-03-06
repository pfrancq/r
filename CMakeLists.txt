cmake_minimum_required(VERSION 2.6)
#if(COMMAND cmake_policy)
 # cmake_policy(SET CMP0003 NEW)
#endif(COMMAND cmake_policy)
PROJECT(r)

CONFIGURE_FILE(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

ADD_CUSTOM_TARGET(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

#force sub libraries link in the good order ... After install, doesn't use the build directory one
#SET(CMAKE_SKIP_RPATH ON)

IF(CMAKE_COMPILER_IS_GNUCC)
	MESSAGE(STATUS "Using GNU C Compiler, set -fmessage-length to 0 (Eclipse support)")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
	MESSAGE(STATUS "Using GNU C++ Compiler, set -fmessage-length to 0 (Eclipse support)")
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmessage-length=0")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

OPTION(disable-mysql "Disable MySQL support" OFF)
OPTION(disable-sqlite "Disable SQLite support" OFF)
OPTION(disable-postgres "Disable PostgreSQL support" OFF)
OPTION(disable-qt "Disable QT4 support" OFF)
OPTIOn(disable-rmath "Disable compiling rmath" OFF)
OPTIOn(disable-roptimization "Disable compiling roptimization" OFF)
OPTIOn(disable-rprg "Disable compiling rprg" OFF)
OPTION(rcmake-verbose "Enable verbose output during cmake construction" OFF)

INCLUDE(rmacro.cmake)

IF(UNIX)
    ADD_DEFINITIONS("-fexceptions")
    MESSAGE(STATUS "C++ exceptions will be re-enabled")
    ADD_DEFINITIONS("-pedantic")
    ADD_DEFINITIONS("-Wno-long-long")
    ADD_DEFINITIONS("-DPIC")
    ADD_DEFINITIONS("-fvisibility=default")
    ADD_DEFINITIONS("-I/usr/include")
ELSE(UNIX)
    ADD_DEFINITIONS("/D_RCORE_DLLAPI_")
ENDIF(UNIX)


#--------------------------------
#path for iconv
# iconv is a main dependency of R so find it or die
SET(ICONVNAME iconv)
IF(WIN32)
    SET(ICONVNAME libiconv)
	IF(MINGW AND NOT DEFINED ${iconv-include})
		SET(iconv-include $ENV{CMAKE_INCLUDE_DIR})
		SET(iconv-library $ENV{CMAKE_LIBRARY_DIR})
	ENDIF(MINGW AND NOT DEFINED ${iconv-include})
ENDIF(WIN32)
LIB_SEARCH_PATH(iconv "${iconv-library}")
INCLUDE_SEARCH_PATH(iconv "${iconv-include}")
FIND_LIBRARY(ICONV_LIB_PATH ${ICONVNAME} ${iconv_LIB_SEARCH_PATH} ${ICONV_LIB_PATH})
FIND_PATH(ICONV_INCLUDE_PATH iconv.h ${iconv_INCLUDE_SEARCH_PATH})
IF(rcmake-verbose)
    MESSAGE(STATUS "iconv library path  configured: lib ${iconv_LIB_SEARCH_PATH} includes ${iconv_INCLUDE_SEARCH_PATH}")
ENDIF(rcmake-verbose)
IF(ICONV_LIB_PATH)
	IF(ICONV_INCLUDE_PATH)
	    INCLUDE_DIRECTORIES(${ICONV_INCLUDE_PATH})
		MESSAGE(STATUS "iconv library successfully configured: lib ${ICONV_LIB_PATH} includes ${ICONV_INCLUDE_PATH}")
	ELSE(ICONV_INCLUDE_PATH)
		MESSAGE(FATAL_ERROR "iconv header files cannot be found.")
	ENDIF(ICONV_INCLUDE_PATH)
ELSE(ICONV_LIB_PATH)
	MESSAGE(STATUS "Unable to find GNU iconv independent library, you may consider to install it ...")
	SET(ICONV_LIB_PATH "")
	IF(ICONV_INCLUDE_PATH)
		MESSAGE(STATUS "Using libc iconv with includes ${ICONV_INCLUDE_PATH}")
	ELSE(ICONV_INCLUDE_PATH)
		MESSAGE(FATAL_ERROR "iconv header files cannot be found.")
	ENDIF(ICONV_INCLUDE_PATH)
ENDIF(ICONV_LIB_PATH)


#--------------------------------
#path for curl
SET(CURLNAME curl)
IF(WIN32)
	IF(MINGW)
		SET(CURLNAME libcurl)
		IF(NOT DEFINED ${curl-include})
			SET(curl-include "C:/curl/include")
		ENDIF(NOT DEFINED ${curl-include})
		IF(NOT DEFINED ${curl-library})
			SET(curl-library "C:/curl/lib")
		ENDIF(NOT DEFINED ${curl-library})
	ELSE(MINGW)
		SET(CURLNAME libcurl_imp)
	ENDIF(MINGW)
ENDIF(WIN32)
LIB_SEARCH_PATH(curl "${curl-library}")
INCLUDE_SEARCH_PATH(curl "${curl-include}")
FIND_LIBRARY(CURL_LIB_PATH ${CURLNAME} ${curl_LIB_SEARCH_PATH})
FIND_PATH(CURL_INCLUDE_PATH curl/curl.h ${curl_INCLUDE_SEARCH_PATH})
IF(rcmake-verbose)
    MESSAGE(STATUS "curl library path configured: lib ${curl_LIB_SEARCH_PATH} includes ${curl_INCLUDE_SEARCH_PATH}")
ENDIF(rcmake-verbose)
IF(CURL_LIB_PATH)
	IF(CURL_INCLUDE_PATH)
	    INCLUDE_DIRECTORIES(${CURL_INCLUDE_PATH})
		MESSAGE(STATUS "CURL library successfully configured: lib ${CURL_LIB_PATH} includes ${CURL_INCLUDE_PATH}")
	ELSE(CURL_INCLUDE_PATH)
		MESSAGE(FATAL_ERROR "CURL header files cannot be found.")
	ENDIF(CURL_INCLUDE_PATH)
ELSE(CURL_LIB_PATH)
	MESSAGE(FATAL_ERROR "CURL library cannot be found.")
ENDIF(CURL_LIB_PATH)


#--------------------------------
#path for mysql
IF(disable-mysql)
    IF(WIN32 AND NOT MINGW)
        MESSAGE(FATAL_ERROR "MySQL support is mandatory under Windows.  Enable MySQL by using -Ddisable-mysql=no")
    ELSE(WIN32 AND NOT MINGW)
	    MESSAGE(STATUS "MySQL support disabled")
	    SET(MYSQLCLIENT_LIB_PATH "")
	ENDIF(WIN32 AND NOT MINGW)
ELSE(disable-mysql)
	IF(${mysql-library})
		MESSAGE(STATUS "coucou")
	ENDIF(${mysql-library})
    LIB_SEARCH_PATH(mysql "${mysql-library}")
    INCLUDE_SEARCH_PATH(mysql "${mysql-include}")
    FIND_LIBRARY(MYSQLCLIENT_LIB_PATH mysqlclient ${mysql_LIB_SEARCH_PATH})
    IF(WIN32)
        FIND_LIBRARY(ZLIBMYSQL_LIB_PATH zlib ${mysql_LIB_SEARCH_PATH})
    ENDIF(WIN32)
	FIND_PATH(MYSQLCLIENT_INCLUDE_PATH mysql/mysql.h ${mysql_INCLUDE_SEARCH_PATH})
	IF(rcmake-verbose)
	    MESSAGE(STATUS "mysql library path configured: lib ${mysql_LIB_SEARCH_PATH} includes ${mysql_INCLUDE_SEARCH_PATH}")
	ENDIF(rcmake-verbose)
	IF(MYSQLCLIENT_LIB_PATH)
		IF(MYSQLCLIENT_INCLUDE_PATH)
			INCLUDE_DIRECTORIES(${MYSQLCLIENT_INCLUDE_PATH})
		    MESSAGE(STATUS "MySQL client library successfully configured: lib ${ZLIBMYSQL_LIB_PATH} ${MYSQLCLIENT_LIB_PATH} includes ${MYSQLCLIENT_INCLUDE_PATH}")
		ELSE(MYSQLCLIENT_INCLUDE_PATH)
			MESSAGE(FATAL_ERROR "MySQL client header files cannot be found.")
		ENDIF(MYSQLCLIENT_INCLUDE_PATH)
	ELSE(MYSQLCLIENT_LIB_PATH)
		MESSAGE(FATAL_ERROR "MySQL client library cannot be found.")
	ENDIF(MYSQLCLIENT_LIB_PATH)
ENDIF(disable-mysql)


#--------------------------------
#path for sqlite
IF(disable-sqlite)
    MESSAGE(STATUS "SQLite support disabled")
    SET(SQLITE_LIB_PATH "")
ELSE(disable-sqlite)
    LIB_SEARCH_PATH(sqlite "${sqlite-library}")
    INCLUDE_SEARCH_PATH(sqlite "${sqlite-include}")
    FIND_LIBRARY(SQLITE_LIB_PATH sqlite3 ${sqlite_LIB_SEARCH_PATH})
	FIND_PATH(SQLITE_INCLUDE_PATH sqlite3.h ${sqlite_INCLUDE_SEARCH_PATH})
	IF(rcmake-verbose)
	    MESSAGE(STATUS "SQLite library path configured: lib ${sqlite_LIB_SEARCH_PATH} includes ${sqlite_INCLUDE_SEARCH_PATH}")
	ENDIF(rcmake-verbose)
	IF(SQLITE_LIB_PATH)
		IF(SQLITE_INCLUDE_PATH)
			INCLUDE_DIRECTORIES(${SQLITE_INCLUDE_PATH})
		    MESSAGE(STATUS "SQLite client library successfully configured: lib ${SQLITE_LIB_PATH} includes ${SQLITE_INCLUDE_PATH}")
		ELSE(SQLITE_INCLUDE_PATH)
			MESSAGE(FATAL_ERROR "SQLite client header files cannot be found.")
		ENDIF(SQLITE_INCLUDE_PATH)
	ELSE(SQLITE_LIB_PATH)
		MESSAGE(FATAL_ERROR "SQLite client library cannot be found.")
	ENDIF(SQLITE_LIB_PATH)
ENDIF(disable-sqlite)


#--------------------------------
#search for PostgreSQL
IF(disable-postgres)
	MESSAGE(STATUS "PostgreSQL support disabled")
ELSE(disable-postgres)
	FIND_PACKAGE(PostgreSQL)
	IF(PostgreSQL_FOUND)
		IF(PostgreSQL_TYPE_INCLUDE_DIR)
		ELSE( PostgreSQL_TYPE_INCLUDE_DIR)
			SET(PostgreSQL_TYPE_INCLUDE_DIR ${PostgreSQL_INCLUDE_DIR})
		ENDIF(PostgreSQL_TYPE_INCLUDE_DIR)
		INCLUDE_DIRECTORIES(${PostgreSQL_INCLUDE_DIRS})
		LINK_DIRECTORIES(${PostgreSQL_LIBRARY_DIRS})
		MESSAGE(STATUS "PostgreSQL support enabled.")
	ELSE(PostgreSQL_FOUND)
		MESSAGE(FATAL_ERROR "PostgreSQL can't be found as requested")
	ENDIF(PostgreSQL_FOUND)
ENDIF(disable-postgres)


#--------------------------------
IF(extra-include)
    INCLUDE_DIRECTORIES(${extra-include})
ENDIF(extra-include)


#--------------------------------
IF(UNIX)
#need thread for using RTHREAD
    FIND_PACKAGE(Threads REQUIRED)
ELSE(UNIX)
    MESSAGE(STATUS "Will use MySQL pthread functions ...")
    ADD_DEFINITIONS("/D__WIN__")
    ADD_DEFINITIONS("/DDBUG_OFF")
ENDIF(UNIX)


#--------------------------------
#search for Qt4
IF(disable-qt)
	MESSAGE(STATUS "Qt4 support disabled")
ELSE(disable-qt)
	FIND_PACKAGE(Qt4 REQUIRED)
	IF(QT4_FOUND)
        INCLUDE(${QT_USE_FILE})
        ADD_DEFINITIONS(${QT_DEFINITIONS})
		INCLUDE_DIRECTORIES( ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )
		MESSAGE(STATUS "Qt4 support enabled.")
	ELSE(QT4_FOUND)
		MESSAGE(FATAL_ERROR "QT4 can't be found as requested")
	ENDIF(QT4_FOUND)
ENDIF(disable-qt)

SET(SUB_PROJECT rcore)
ADD_SUBDIRECTORY(rcore)

IF(disable-rmath)
	MESSAGE(STATUS "rmath disabled")
ELSE(disable-rmath)
	SET(SUB_PROJECT ${SUB_PROJECT} rmath)
	ADD_SUBDIRECTORY(rmath)
ENDIF(disable-rmath)

IF(disable-roptimization)
	MESSAGE(STATUS "roptimization disabled")
ELSE(disable-roptimization)
	SET(SUB_PROJECT ${SUB_PROJECT} roptimization)
	ADD_SUBDIRECTORY(roptimization)
ENDIF(disable-roptimization)

IF(disable-rprg)
	MESSAGE(STATUS "rprg disabled")
ELSE(disable-rprg)
	SET(SUB_PROJECT ${SUB_PROJECT} rprg)
	ADD_SUBDIRECTORY(rprg)
ENDIF(disable-rprg)

PRINT_LIST_WITH_MESSAGE("R Library is now configured, the following part will be constructed:" SUB_PROJECT)

###file to install
##keep this macro for r based project in a standard place
INSTALL(FILES rmacro.cmake DESTINATION share/r)
##keep it for compatibility with auto tools...
#INSTALL(FILES libr.m4 DESTINATION share/aclocal)


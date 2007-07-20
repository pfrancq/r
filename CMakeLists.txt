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
OPTION(disable-kde "Disable KDE 3 support" OFF)
OPTION(rcmake-verbose "Enable verbose output during cmake construction" OFF)

INCLUDE(rmacro.cmake)

ADD_DEFINITIONS("-fexceptions")
MESSAGE(STATUS "C++ exceptions will be re-enabled (since KDE3 detection disable them)")
ADD_DEFINITIONS("-pedantic")
ADD_DEFINITIONS("-DPIC")

##set some vars
#path for iconv
LIB_SEARCH_PATH(iconv "${iconv-library}")
INCLUDE_SEARCH_PATH(iconv "${iconv-include}")
#iconv is a main dependency of R so find it or die
FIND_LIBRARY(ICONV_LIB_PATH iconv iconv_LIB_SEARCH_PATH)
FIND_PATH(ICONV_INCLUDE_PATH iconv.h iconv_INCLUDE_SEARCH_PATH)
IF(ICONV_LIB_PATH)
	IF(ICONV_INCLUDE_PATH)
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


#path for mysql
IF(disable-mysql)
	MESSAGE(STATUS "MySQL support disabled")
	SET(MYSQLCLIENT_LIB_PATH "")
ELSE(disable-mysql)
    LIB_SEARCH_PATH(mysql "${mysql-library}")
    INCLUDE_SEARCH_PATH(mysql "${mysql-include}")
	FIND_LIBRARY(MYSQLCLIENT_LIB_PATH mysqlclient ${MYSQL_LIB_SEARCH_PATH})
	FIND_PATH(MYSQLCLIENT_INCLUDE_PATH mysql/mysql.h ${MYSQL_INCLUDE_SEARCH_PATH})
	IF(MYSQLCLIENT_LIB_PATH)
		IF(MYSQLCLIENT_INCLUDE_PATH)
			INCLUDE_DIRECTORIES(${MYSQLCLIENT_INCLUDE_PATH})
		    MESSAGE(STATUS "MySQL client library successfully configured: lib ${MYSQLCLIENT_LIB_PATH} includes ${MYSQLCLIENT_INCLUDE_PATH}")
		ELSE(MYSQLCLIENT_INCLUDE_PATH)
			MESSAGE(FATAL_ERROR "MySQL client header files cannot be found.")
		ENDIF(MYSQLCLIENT_INCLUDE_PATH)	
	ELSE(MYSQLCLIENT_LIB_PATH)
		MESSAGE(FATAL_ERROR "MySQL client library cannot be found.")
	ENDIF(MYSQLCLIENT_LIB_PATH)
ENDIF(disable-mysql)

#path for curl
LIB_SEARCH_PATH(curl "${curl-library}")
INCLUDE_SEARCH_PATH(curl "${curl-include}")
FIND_LIBRARY(CURL_LIB_PATH curl /usr/lib /usr/local/lib)
FIND_PATH(CURL_INCLUDE_PATH curl.h /usr/include /usr/include/curl /usr/lcoal/include /usr/local/include/curl)
IF(CURL_LIB_PATH)
	IF(CURL_INCLUDE_PATH)
		MESSAGE(STATUS "CURL library successfully configured: lib ${CURL_LIB_PATH} includes ${CURL_INCLUDE_PATH}")
	ELSE(CURL_INCLUDE_PATH)
		MESSAGE(FATAL_ERROR "CURL header files cannot be found.")
	ENDIF(CURL_INCLUDE_PATH)	
ELSE(CURL_LIB_PATH)
	MESSAGE(FATAL_ERROR "CURL library cannot be found.")
ENDIF(CURL_LIB_PATH)

#need thread for using RTHREAD
FIND_PACKAGE(Threads REQUIRED)

#search for kde3
IF(disable-kde)
	MESSAGE(STATUS "KDE3 support disabled")
ELSE(disable-kde)
	FIND_PACKAGE(KDE3 REQUIRED)
	IF(KDE3_FOUND)
		ADD_DEFINITIONS(${QT_DEFINITIONS} ${KDE3_DEFINITIONS})
		INCLUDE_DIRECTORIES( ${KDE3_INCLUDE_DIR} ${QT_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )
		MESSAGE(STATUS "KDE3 support enabled.")
	ELSE(KDE3_FOUND)
		MESSAGE(FATAL_ERROR "KDE3 can't be found as requested")
	ENDIF(KDE3_FOUND)
ENDIF(disable-kde)

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

PRINT_LIST_WITH_MESSAGE("R Library is now configured, the following part will be constructed:" SUB_PROJECT)

###file to install
##keep this macro for r based project in a standard place
INSTALL(FILES rmacro.cmake DESTINATION share/r)
##keep it for compatibility with auto tools...
INSTALL(FILES libr.m4 DESTINATION share/aclocal)


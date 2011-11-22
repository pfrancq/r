IF(disable-mysql)
    SET(MYSQL_INCLUDES "")
    SET(MYSQL_SOURCES "")
ELSE(disable-mysql)
    SET(MYSQL_INCLUDES rdbmysql.h)
    SET(MYSQL_SOURCES rdbmysql.cpp)
ENDIF(disable-mysql)

IF(disable-sqlite)
    SET(SQLITE_INCLUDES "")
    SET(SQLITE_SOURCES "")
ELSE(disable-sqlite)
    SET(SQLITE_INCLUDES rdbsqlite.h)
    SET(SQLITE_SOURCES rdbsqlite.cpp)
ENDIF(disable-sqlite)

IF(disable-postgres)
    SET(POSTGRES_INCLUDES "")
    SET(POSTGRES_SOURCES "")
ELSE(disable-postgres)
    SET(POSTGRES_INCLUDES rdbpostgres.h)
    SET(POSTGRES_SOURCES rdbpostgres.cpp)
ENDIF(disable-postgres)


SET(rcore_db_INST_INCLUDES rdb.h rquery.h rtransactiontable.h ${MYSQL_INCLUDES} ${SQLITE_INCLUDES} ${POSTGRES_INCLUDES})
SET(rcore_db_TARGET_SOURCES rdb.cpp rquery.cpp rtransactiontable.cpp ${MYSQL_SOURCES} ${SQLITE_SOURCES} ${POSTGRES_SOURCES})


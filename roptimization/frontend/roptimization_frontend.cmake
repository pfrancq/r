IF(QT4_FOUND)
    SET(roptimization_AVAILABLE_FRONTENDS ${rcore_AVAILABLE_FRONTENDS} qt)
    SET(roptimization_qt_DEPENDENCY rcoreqt ${QT_LIBRARIES})
    ADD_FRONTEND_SOURCE_FILE(roptimization_frontend roptimization qt ROPTIMIZATION_INCLUDES)
ENDIF(QT4_FOUND)

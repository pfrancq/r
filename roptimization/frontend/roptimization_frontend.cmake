IF(KDE3_FOUND)	
    SET(roptimization_AVAILABLE_FRONTENDS ${rcore_AVAILABLE_FRONTENDS} kde)
    SET(roptimization_kde_DEPENDENCY rcorekde ${QT_AND_KDECORE_LIBS})
    ADD_FRONTEND_SOURCE_FILE(roptimization_frontend roptimization kde ROPTIMIZATION_INCLUDES)
ENDIF(KDE3_FOUND)
  
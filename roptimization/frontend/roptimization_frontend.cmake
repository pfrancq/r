IF(KDE4_FOUND)	
    SET(roptimization_AVAILABLE_FRONTENDS ${rcore_AVAILABLE_FRONTENDS} kde)
    SET(roptimization_kde_DEPENDENCY rcorekde ${KDE4_KDEUI_LIBS})
    ADD_FRONTEND_SOURCE_FILE(roptimization_frontend roptimization kde ROPTIMIZATION_INCLUDES)
ENDIF(KDE4_FOUND)
  
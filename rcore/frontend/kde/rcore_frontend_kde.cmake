SET(rcore_frontend_kde_MOC_INCLUDES rqconsole.h)

SET(rcore_frontend_kde_UI_INCLUDES )

SET(rcore_frontend_kde_INST_INCLUDES rqt.h
                                                          rqconsole.h)

SET(rcore_frontend_kde_TARGET_SOURCES rqt.cpp
                                                              rqconsole.cpp)

                                                              
###This is not very simple way to produce moc file
## A macro must be added
FOREACH(file ${rcore_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})
    KDE3_ADD_MOC_FILES(rcore_frontend_kde_MOCFILE ${REALFILE})
ENDFOREACH(file ${rcore_frontend_kde_MOC_INCLUDES})

FOREACH(file ${rcore_frontend_kde_UI_INCLUDES})
    SET(REALFILE frontend/kde/${file})
   KDE3_ADD_UI_FILES(rcore_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${rcore_frontend_kde_UI_INCLUDES})
                                                              
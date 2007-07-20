SET(roptimization_frontend_kde_MOC_INCLUDES qpromcriteria.h 
                                            qxmlcontainer.h 
                                            qgamonitor.h)
                                             
SET(roptimization_frontend_kde_INST_INCLUDES ${roptimization_frontend_kde_MOC_INCLUDES}
                                             qdrawpolygons.h)
                                     
SET(roptimization_frontend_kde_TARGET_SOURCES qgamonitor.cpp 
                                              qpromcriteria.cpp 
                                              qxmlcontainer.cpp 
                                              qdrawpolygons.cpp)

###This is not very simple way to produce moc file
## A macro must be added                              
FOREACH(file ${roptimization_frontend_kde_MOC_INCLUDES})
    SET(REALFILE frontend/kde/${file})     
    KDE3_ADD_MOC_FILES(roptimization_frontend_kde_MOCFILE ${REALFILE})                                           
ENDFOREACH(file ${roptimization_frontend_kde_MOC_INCLUDES})

#SET(roptimization_TARGET_SOURCES ${roptimization_TARGET_SOURCES} ${roptimization_MOCFILE})
#SET(roptimization_frontend_kde_TARGET_SOURCES )   
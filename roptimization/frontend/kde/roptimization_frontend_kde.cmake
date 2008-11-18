SET(roptimization_frontend_kde_INST_INCLUDES
qpromcriteria.h 
qxmlcontainer.h 
qgamonitor.h
qdrawpolygons.h
)
                                     
SET(roptimization_frontend_kde_TARGET_SOURCES
qgamonitor.cpp 
qpromcriteria.cpp 
qxmlcontainer.cpp 
qdrawpolygons.cpp
)

#KDE4_ADD_UI_FILES(roptimization_frontend_kde_TARGET_SOURCES )

#KDE4_ADD_KCFG_FILES(roptimization_frontend_kde_TARGET_SOURCES )
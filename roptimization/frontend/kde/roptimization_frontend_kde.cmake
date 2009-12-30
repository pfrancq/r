SET(roptimization_frontend_kde_INST_INCLUDES
	qrgamonitor.h
	qrdebug.h 
	qrdrawgeoinfos.h
)
                                     
SET(roptimization_frontend_kde_TARGET_SOURCES
	qrgamonitor.cpp 
	qrdebug.cpp 
	qrdrawgeoinfos.cpp
)

SET(roptimization_frontend_kde_UI_INCLUDES
	qrdrawgeoinfos.ui
	qrdebug.ui
	qrgamonitor.ui
)


FOREACH(file ${roptimization_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(roptimization_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${roptimization_frontend_kde_UI_INCLUDES})
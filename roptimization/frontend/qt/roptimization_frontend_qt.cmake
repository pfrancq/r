SET(roptimization_frontend_qt_INST_INCLUDES
	qrgamonitor.h
	qrdebug.h
	qrdrawgeoinfos.h
	qrpromcriterion.h
)

SET(roptimization_frontend_qt_TARGET_SOURCES
	qrgamonitor.cpp
	qrdebug.cpp
	qrdrawgeoinfos.cpp
	qrpromcriterion.cpp
)

SET(roptimization_frontend_qt_UI_INCLUDES
	qrdrawgeoinfos.ui
	qrdebug.ui
	qrgamonitor.ui
	qrpromcriterion.ui
)


FOREACH(file ${roptimization_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(roptimization_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${roptimization_frontend_qt_UI_INCLUDES})

FOREACH(file ${roptimization_frontend_qt_INST_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(roptimization_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${roptimization_frontend_qt_INST_INCLUDES})
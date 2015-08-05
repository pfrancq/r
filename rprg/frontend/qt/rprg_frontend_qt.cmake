SET(rprg_frontend_qt_INST_INCLUDES
	qrconsole.h
)


SET(rprg_frontend_qt_Qt_INCLUDES
	qrconsole.h
)


SET(rprg_frontend_qt_TARGET_SOURCES
	qrconsole.cpp
)

SET(rprg_frontend_qt_UI_INCLUDES
)


FOREACH(file ${rprg_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(rprg_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${rprg_frontend_qt_UI_INCLUDES})

FOREACH(file ${rprg_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(rprg_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${rprg_frontend_qt_Qt_INCLUDES})

SET(rcore_frontend_qt_INST_INCLUDES
rqt.h
qrconsole.h
qrxmlstruct.h
qrpainter.h
)


SET(rcore_frontend_qt_Qt_INCLUDES
qrconsole.h
qrxmlstruct.h
qrpainter.h
)


SET(rcore_frontend_qt_TARGET_SOURCES
rqt.cpp
qrconsole.cpp
qrxmlstruct.cpp
qrpainter.cpp
)

SET(rcore_frontend_qt_UI_INCLUDES
	qrxmlstruct.ui
)


FOREACH(file ${rcore_frontend_qt_UI_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_UI(rcore_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${rcore_frontend_qt_UI_INCLUDES})

FOREACH(file ${rcore_frontend_qt_Qt_INCLUDES})
   SET(REALFILE frontend/qt/${file})
   QT4_WRAP_CPP(rcore_frontend_qt_UIFILE ${REALFILE})
ENDFOREACH(file ${rcore_frontend_qt_Qt_INCLUDES})


# INSTALL(FILES frontend/kde/nokonqueror.png
# 		frontend/kde/xml_cdata_b.png
# 		frontend/kde/xml_comment_b.png
# 		frontend/kde/xml_contents_b.png
# 		frontend/kde/xml_element_b.png
# 		frontend/kde/xml_procinstr_b.png
# 		frontend/kde/xml_text_b.png
# 		frontend/kde/xml_attribute.png
# 		frontend/kde/xml_cdata.png
# 		frontend/kde/xml_comment.png
# 		frontend/kde/xml_contents.png
# 		frontend/kde/xml_element.png
# 		frontend/kde/xml_procinstr.png
# 		frontend/kde/xml_text.png
#
# 		DESTINATION share/icons/hicolor/16x16/actions/)

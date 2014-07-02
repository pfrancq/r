SET(rcore_frontend_kde_INST_INCLUDES
rqt.h
qrconsole.h
qrxmlstruct.h
qrpainter.h
)

SET(rcore_frontend_kde_TARGET_SOURCES
rqt.cpp
qrconsole.cpp
qrxmlstruct.cpp
qrpainter.cpp
)

SET(rcore_frontend_kde_UI_INCLUDES qrxmlstruct.ui)


FOREACH(file ${rcore_frontend_kde_UI_INCLUDES})
   SET(REALFILE frontend/kde/${file})
   KDE4_ADD_UI_FILES(rcore_frontend_kde_UIFILE ${REALFILE})
ENDFOREACH(file ${rcore_frontend_kde_UI_INCLUDES})

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

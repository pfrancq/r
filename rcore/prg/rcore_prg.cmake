SET(rcore_prg_INST_INCLUDES rprgfunc.h 
                       rprginst.h 
                       rprgvarliteral.h 
                       rprgvarref.h
                       rprgvarinst.h
		               rprg.h 
		               rprgclass.h 
		               rprginstfor.h
		               rprginstassign.h
		               rprginstblock.h
		               rprginstdelete.h  
		               rprgoutput.h
		               rprginstmethod.h 
		               rprgvar.h 
		               rprgvarstring.h)
		               
SET(rcore_prg_TARGET_SOURCES rprgfunc.cpp 
                          rprginstfor.cpp
                          rprginstassign.cpp
                          rprginstblock.cpp
		                  rprgvarliteral.cpp 
		                  rprgvarref.cpp
		                  rprgvarinst.cpp 
		                  rprg.cpp 
		                  rprgoutput.cpp
		                  rprgclass.cpp 
		                  rprginst.cpp
		                  rprginstdelete.cpp 
		                  rprginstmethod.cpp 
		                  rprgvar.cpp 
		                  rprgvarstring.cpp)
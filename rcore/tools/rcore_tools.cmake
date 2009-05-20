SET(rcore_tools_INST_INCLUDES rstd.h
                       rcontainer.h
                       rcontainer.hh
                       rstack.h
                       rcursor.h
                       rhashcontainer.h
                       rdblhashcontainer.h
                       rnode.h
                       rnode.hh
                       rtree.h
                       rtree.hh
                       rshareddata.h
                       basiccontainer.h
                       basiccursor.h
                       basicstring.h
                       basicstring.hh
                       rportsmacro.h
                       rnumcontainer.h
                       rnumcontainer.hh
                       rnumcursor.h 
                       rnumcursor.hh
	                   rtrace.h
                       rstring.h
                       rchar.h
    				   rrandom.h
                        rdate.h
    					rlang.h
    					 rlocale.h
    					 rtextencoding.h
    					 rcstring.h
    					 rvectorbool.h
    					 rvalue.h
)

SET(rcore_tools_TARGET_SOURCES random.cpp
                               rdate.cpp
                               rlang.cpp
                               rlocale.cpp
                               rtrace.cpp
                               rstring.cpp
                               rchar.cpp
                               rtextencoding.cpp
                               rcstring.cpp
                               basiccontainer.cpp
                               basiccursor.cpp
                               rstd.cpp
                               rvectorbool.cpp
                               rvalue.cpp
)
###This macro is provided on CMake site but not in the standard configuration
###User contributed macro
# from http://www.cmake.org/Wiki/CMakeMacroLibtoolFile
MACRO(GET_TARGET_PROPERTY_WITH_DEFAULT _variable _target _property _default_value)
  GET_TARGET_PROPERTY (${_variable} ${_target} ${_property})
  IF (${_variable} STREQUAL NOTFOUND)
    SET (${_variable} ${_default_value})
  ENDIF (${_variable} STREQUAL NOTFOUND)
ENDMACRO (GET_TARGET_PROPERTY_WITH_DEFAULT)

###This macro is provided on CMake site but not in the standard configuration
###User contributed macro, correct compilation path
# from http://www.cmake.org/Wiki/CMakeMacroLibtoolFile  
MACRO(CREATE_LIBTOOL_FILE _target _install_DIR)
  GET_TARGET_PROPERTY(_target_location ${_target} LOCATION)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_static_lib ${_target} STATIC_LIB "")
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_dependency_libs ${_target} LT_DEPENDENCY_LIBS "")
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_current ${_target} LT_VERSION_CURRENT 0)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_age ${_target} LT_VERSION_AGE 0)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_revision ${_target} LT_VERSION_REVISION 0)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_installed ${_target} LT_INSTALLED yes)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_shouldnotlink ${_target} LT_SHOULDNOTLINK yes)
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_dlopen ${_target} LT_DLOPEN "")
  GET_TARGET_PROPERTY_WITH_DEFAULT(_target_dlpreopen ${_target} LT_DLPREOPEN "")
  GET_FILENAME_COMPONENT(_laname ${_target_location} NAME_WE)
  GET_FILENAME_COMPONENT(_soname ${_target_location} NAME)
  SET(_laname ${PROJECT_BINARY_DIR}/${_laname}.la)
  FILE(WRITE ${_laname} "# ${_laname} - a libtool library file, generated by cmake \n")
  FILE(APPEND ${_laname} "# Generated by ltmain.sh - GNU libtool ")
  FILE(APPEND ${_laname} "# Generated by libtool - GNU libtool ")
  FILE(APPEND ${_laname} "# The name that we can dlopen(3).\n")
  FILE(APPEND ${_laname} "dlname='${_soname}'\n")
  FILE(APPEND ${_laname} "# Names of this library\n")
  FILE(APPEND ${_laname} "library_names='${_soname}.${_target_current}.${_target_age}.${_target_revision} ${_soname}.${_target_current} ${_soname}'\n")
  FILE(APPEND ${_laname} "# The name of the static archive\n")
  FILE(APPEND ${_laname} "old_library='${_target_static_lib}'\n")
  FILE(APPEND ${_laname} "# Libraries that this one depends upon.\n")
  FILE(APPEND ${_laname} "dependency_libs='${_target_dependency_libs}'\n")
  FILE(APPEND ${_laname} "# Version information.\n")
  FILE(APPEND ${_laname} "current=${_target_current}\n")
  FILE(APPEND ${_laname} "age=${_target_age}\n")
  FILE(APPEND ${_laname} "revision=${_target_revision}\n")
  FILE(APPEND ${_laname} "# Is this an already installed library?\n")
  FILE(APPEND ${_laname} "installed=${_target_installed}\n")
  #FILE(APPEND ${_laname} "# Should we warn about portability when linking against -modules?\n")
  #FILE(APPEND ${_laname} "shouldnotlink=${_target_shouldnotlink}\n")
  FILE(APPEND ${_laname} "# Files to dlopen/dlpreopen\n")
  FILE(APPEND ${_laname} "dlopen='${_target_dlopen}'\n")
  FILE(APPEND ${_laname} "dlpreopen='${_target_dlpreopen}'\n")
  FILE(APPEND ${_laname} "# Directory that this library needs to be installed in:\n")
  FILE(APPEND ${_laname} "libdir='${CMAKE_INSTALL_PREFIX}/${_install_DIR}'\n")
  INSTALL( FILES ${_laname} ${PROJECT_BINARY_DIR}/${_soname}
    DESTINATION ${CMAKE_INSTALL_PREFIX}${_install_DIR})
ENDMACRO(CREATE_LIBTOOL_FILE)

###This macro define standard search path for a library and add the user command lime cache configuration param
MACRO(LIB_SEARCH_PATH _lib_name _extra_lib_path)
    SET(${_lib_name}_LIB_SEARCH_PATH /usr/lib /usr/local/lib /usr/lib64 /usr/local/lib64 /usr/lib32 /usr/local/lib32)
    IF(NOT ${_extra_lib_path} STREQUAL "")
	    SET(${_lib_name}_LIB_SEARCH_PATH ${_extra_lib_path} ${${_lib_name}_LIB_SEARCH_PATH})
    ENDIF(NOT ${_extra_lib_path} STREQUAL "")
ENDMACRO(LIB_SEARCH_PATH)

###This macro define standard search path for includes file and add the user command lime cache configuration param
MACRO(INCLUDE_SEARCH_PATH _include_name _extra_include_path)
    SET(${_include_name}_INCLUDE_SEARCH_PATH /usr/include /usr/local/include)
    IF(NOT ${_extra_include_path} STREQUAL "")
	    SET(${_include_name}_INCLUDE_SEARCH_PATH ${_extra_include_path} ${${_include_name}_INCLUDE_SEARCH_PATH})
    ENDIF(NOT ${_extra_include_path} STREQUAL "")
ENDMACRO(INCLUDE_SEARCH_PATH)

MACRO(SUBDIR_PATH_MANIPULATION)
    IF(R_CUR_PATH)
        #MESSAGE(STATUS "Current path for sub dir is ${R_CUR_PATH}")
    ELSE(R_CUR_PATH)
        SET(R_CUR_PATH ${PROJECT_SOURCE_DIR})
    ENDIF(R_CUR_PATH)
    
    #save path for recursion
    SET(TMP_R_CUR_PATH ${R_CUR_PATH})
    
    #set path to current directory
    SET(R_CUR_PATH ${R_CUR_PATH}/${_in_dir})
ENDMACRO(SUBDIR_PATH_MANIPULATION)

###Add source file to a target when defined in sub dirs.
## in each subdirs, add a cmake file with the name ${current_target}_current_path_to_dir_from_target_dir_with_underscore_in_place_of_path_separator.cmake
## in this file, define the source list with a SET(${current_target}_current_path_to_dir_from_target_dir_with_underscore_in_place_of_path_separator_TARGET_SOURCES list of SOURCE FILES)
## also define the header list for installation with a SET(${current_target}_current_path_to_dir_from_target_dir_with_underscore_in_place_of_path_separator_INCLUDES list of HEADER FILES)
MACRO(ADD_SOURCE_FILE _for_target _in_dir _includes)
    #check the source base path or set the project source dir if not already set
    IF(R_CUR_PATH)
        #MESSAGE(STATUS "Current path for sub dir is ${R_CUR_PATH}")
    ELSE(R_CUR_PATH)
        SET(R_CUR_PATH ${PROJECT_SOURCE_DIR})
    ENDIF(R_CUR_PATH)
    
    #save path for recursion
    SET(TMP_R_CUR_PATH ${R_CUR_PATH})
    
    #set path to current directory
    SET(R_CUR_PATH ${R_CUR_PATH}/${_in_dir})
    SET(${_includes} ${${_includes}} ${R_CUR_PATH})
    
    #include sub directory content
    INCLUDE(${R_CUR_PATH}/${_for_target}_${_in_dir}.cmake)
    
    #for each source file, add it to the list of file to build
    FOREACH(${_for_target}_${_in_dir}file ${${_for_target}_${_in_dir}_TARGET_SOURCES})
	    SET(${_for_target}_TARGET_SOURCES ${${_for_target}_TARGET_SOURCES} ${_in_dir}/${${_for_target}_${_in_dir}file})
    ENDFOREACH(${_for_target}_${_in_dir}file)
    
    #for each include file, add it to the list of file to install
    FOREACH(${_for_target}_${_in_dir}file ${${_for_target}_${_in_dir}_INST_INCLUDES})
	    SET(${_for_target}_INST_INCLUDES ${${_for_target}_INST_INCLUDES} ${_in_dir}/${${_for_target}_${_in_dir}file})
    ENDFOREACH(${_for_target}_${_in_dir}file)
    
    #restore path after include (and recursion)
    SET(R_CUR_PATH ${TMP_R_CUR_PATH})
ENDMACRO(ADD_SOURCE_FILE)

###Add source file to a frontend target when defined in sub dirs.
## in each subdirs, add a cmake file with the name ${current_target}_current_path_to_dir_from_target_dir_with_underscore_in_place_of_path_separator.cmake
## in this file, define the source list with a SET(${current_target}_current_path_to_dir_from_target_dir_with_underscore_in_place_of_path_separator_TARGET_SOURCES list of SOURCE FILES)
## also define the header list for installation with a SET(${current_target}_current_path_to_dir_from_target_dir_with_underscore_in_place_of_path_separator_INCLUDES list of HEADER FILES)
MACRO(ADD_FRONTEND_SOURCE_FILE _for_target _real_target _in_dir _includes)
    #check the source base path or set the project source dir if not already set
    IF(R_CUR_PATH)
        #MESSAGE(STATUS "Current path for sub dir is ${R_CUR_PATH}")
    ELSE(R_CUR_PATH)
        SET(R_CUR_PATH ${PROJECT_SOURCE_DIR})
    ENDIF(R_CUR_PATH)
    
    #save path for recursion
    SET(TMP_R_CUR_PATH ${R_CUR_PATH})
    
    #set path to current directory
    SET(R_CUR_PATH ${R_CUR_PATH}/${_in_dir})
    SET(${_includes} ${${_includes}} ${R_CUR_PATH})
    
    #include sub directory content
    INCLUDE(${R_CUR_PATH}/${_for_target}_${_in_dir}.cmake)
    
    #for each source file, add it to the list of file to build
    FOREACH(${_for_target}_${_in_dir}file ${${_for_target}_${_in_dir}_TARGET_SOURCES})
        SET(${_real_target}_${_in_dir}_TARGET_SOURCES ${${_real_target}_${_in_dir}_TARGET_SOURCES} ${R_CUR_PATH}/${${_for_target}_${_in_dir}file})
    ENDFOREACH(${_for_target}_${_in_dir}file)
    
    #for each moc file, add it to the list of file to build
    FOREACH(${_for_target}_${_in_dir}file ${${_for_target}_${_in_dir}_MOCFILE})
        SET(${_real_target}_${_in_dir}_TARGET_SOURCES ${${_real_target}_${_in_dir}_TARGET_SOURCES} ${${_for_target}_${_in_dir}file})
    ENDFOREACH(${_for_target}_${_in_dir}file)
    
    #for each include file, add it to the list of file to install
    FOREACH(${_for_target}_${_in_dir}file ${${_for_target}_${_in_dir}_INST_INCLUDES})
	    SET(${_real_target}_${_in_dir}_INST_INCLUDES ${${_real_target}_${_in_dir}_INST_INCLUDES} ${R_CUR_PATH}/${${_for_target}_${_in_dir}file})
    ENDFOREACH(${_for_target}_${_in_dir}file)
    
    #restore path after include (and recursion)
    SET(R_CUR_PATH ${TMP_R_CUR_PATH})
ENDMACRO(ADD_FRONTEND_SOURCE_FILE)

###this macro is usefull to produce list output 
MACRO(PRINT_LIST_WITH_MESSAGE _message _values)
    MESSAGE(STATUS ${_message})
    FOREACH(part ${${_values}})
        MESSAGE(STATUS "    ${part}")
    ENDFOREACH(part ${${_values}})
ENDMACRO(PRINT_LIST_WITH_MESSAGE _message _values)

###this macro prepare compilation of the selected frontend
MACRO(ADD_FRONTEND _frontend_for_target _lt_vers _so_vers _vers _libdest _include_dest _includes)
    FOREACH(${_frontend_for_target}_fe ${${_frontend_for_target}_AVAILABLE_FRONTENDS})
        INCLUDE_DIRECTORIES(${${_includes}})
        ADD_LIBRARY(${_frontend_for_target}${${_frontend_for_target}_fe} SHARED ${${_frontend_for_target}_${${_frontend_for_target}_fe}_TARGET_SOURCES})
        TARGET_LINK_LIBRARIES(${_frontend_for_target}${${_frontend_for_target}_fe} ${_frontend_for_target} ${${_frontend_for_target}_${${_frontend_for_target}_fe}_DEPENDENCY})
        SET_TARGET_PROPERTIES(${_frontend_for_target}${${_frontend_for_target}_fe} PROPERTIES LT_VERSION_CURRENT ${_lt_vers}
                                               VERSION ${_vers}
                                               SOVERSION ${_so_vers})
        CREATE_LIBTOOL_FILE(${_frontend_for_target}${${_frontend_for_target}_fe} /${_libdest})
        INSTALL(TARGETS ${_frontend_for_target}${${_frontend_for_target}_fe} LIBRARY DESTINATION ${_libdest})
        INSTALL(FILES ${${_frontend_for_target}_${${_frontend_for_target}_fe}_INST_INCLUDES} DESTINATION include/${_include_dest})
        IF(rcmake-verbose)
            PRINT_LIST_WITH_MESSAGE("Will install the following header files for rcore${${_frontend_for_target}_fe}:" ${_frontend_for_target}_${${_frontend_for_target}_fe}_INST_INCLUDES)
        ENDIF(rcmake-verbose) 
    ENDFOREACH(${_frontend_for_target}_fe ${${_frontend_for_target}_AVAILABLE_FRONTENDS})
ENDMACRO(ADD_FRONTEND _frontend_for_target _lt_vers _so_vers _vers _libdest _include_dest _type _includes)
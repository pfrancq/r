#ifndef __RWINEXPORT_H__
#define __RWINEXPORT_H__

#ifdef WIN32
#pragma warning(disable:4800) //remove the int -> bool perfomance warning ...
#pragma warning(disable:4996) //remove deprecated functions warning ...

#define _RCORE_NOAUTOLIB_
//see http://www.codeproject.com/dll/dllexport.asp

// The following will ensure that we are exporting our C++ classes when 
// building the DLL and importing the classes when build an application 
// using this DLL.

#ifdef _RCORE_DLLAPI_
    #define RCORE_DLLAPI  __declspec( dllexport )
#else
    #define RCORE_DLLAPI  __declspec( dllimport )
#endif

// The following will ensure that when building an application (or another
// DLL) using this DLL, the appropriate .LIB file will automatically be used
// when linking.

#ifndef _RCORE_NOAUTOLIB_
#ifdef _DEBUG
#pragma comment(lib, "rcored.lib")
#else
#pragma comment(lib, "rcore.lib")
#endif
#endif //_RCORE_NOAUTOLIB_
#else //WIN32
#define RCORE_DLLAPI 
#endif //WIN32
#endif //__RWINEXPORT_H__

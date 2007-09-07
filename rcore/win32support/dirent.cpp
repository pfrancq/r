#include "dirent.h"

int mkdir(const char *path, int flags){
	return -1;
}

DIR * opendir (const char *name){
	char tmpName[2048];
	GetCurrentDirectory(2048, tmpName);

	SetCurrentDirectory (name);
	CURHANDLE = FindFirstFile("*.*", &lastfile);
	if(CURHANDLE == INVALID_HANDLE_VALUE)
		return 0;
	SetCurrentDirectory (tmpName);

	firstcall=1;

	return &CURHANDLE;
}

int closedir (DIR *dirp){
	FindClose(*dirp);
	return 0;
}

struct dirent * readdir (DIR *dirp){
	if(firstcall){
		firstcall = 0;
		lastdirentfile.d_name = lastfile.cFileName;
		return &lastdirentfile;
	}

	if(FindNextFile(*dirp, &lastfile)){
		lastdirentfile.d_name = lastfile.cFileName;
		return &lastdirentfile;
	}

	return 0;
}
#ifndef __DIRENT_H__
#define __DIRENT_H__

#include <windows.h>

//on windows, groups are not supported (and other also), so apply the same user rights
#define S_IXGRP _S_IEXEC  /* Execute by group. not supported on windows, so fake it */
#define S_IXOTH _S_IEXEC  /* Execute by others. not supported on windows, so fake it */
#define S_IRGRP _S_IREAD  /* Read by group. not supported on windows, so fake it */
#define S_IROTH _S_IREAD  /* Read by others. not supported on windows, so fake it */
//**** taken from linux sys/stat.h
#define S_IRWXU (_S_IREAD|_S_IWRITE|_S_IEXEC)
#define _S_ISTYPE(mode, mask)  (((mode) & _S_IFMT) == (mask))
#define S_ISDIR(mode)    _S_ISTYPE((mode), _S_IFDIR)
//****

int mkdir(const char *path, int flags);

//prototype partially taken from GNU libc
typedef HANDLE DIR;
struct dirent
{
char *d_name;		/* Variable length.  */
//int d_fileno;
};


static int firstcall;
static WIN32_FIND_DATA lastfile;
static DIR CURHANDLE;
static struct dirent lastdirentfile;

DIR * opendir (const char *name);
int closedir (DIR *dirp);
struct dirent * readdir (DIR *dirp);
#endif //__DIRENT_H__
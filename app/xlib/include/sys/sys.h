#ifndef __SYS_H
#define __SYS_H

#define ATTR_ARCHIVE 	0
#define ATTR_DIRECTORY 	1



#include "diskblock.h"

#include <ctype.h>
#include <fs/fat.h>


extern char **__argv;
extern int __argc;
#define __argv __argv
#define __argc __argc


FILE *open(const char *path,int attr,const char *mode);
int close(FILE *fd);
int flush(FILE *fp);

int creat(const char *path,int attr);
int rmove (const char *path);




int _putc (int ch, FILE *fp);
int _getc (FILE *fp);

int _fputs (const char *str,FILE *fp);
char *_fgets (char *str,int length,FILE *fp);



#endif

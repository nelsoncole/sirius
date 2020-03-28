#ifndef __SYS_H
#define __SYS_H

#define ATTR_ARCHIVE 	0
#define ATTR_DIRECTORY 	1


// Mensagens xserver

#define	X_MSG_EXIT 	0x1
#define	X_MSG_WAIT	0x2
#define X_MSG_EXEC	0x3
#define	X_MSG_CREATE	0x4
#define	X_MSG_DELETE	0x5
#define	X_MSG_COPY	0x6
#define	X_MSG_MOVE	0x7
#define	X_MSG_REBOOT	0x8
#define	X_MSG_SHUTDOWN	0x9
#define	X_MSG_WAIT_PID	0x71
#define	X_MSG_INT	0x72


#include <typ.h>
#include "diskblock.h"

#include <ctype.h>
#include <fs/fat.h>

#include <stdx/stdx.h>

extern FILE *stdx;
#define stdx stdx

extern FILE *stdsd;
extern FILE *stdxserver;
extern FILE *stdgserver;

#define stdsd stdsd
#define stdxserver stdxserver
#define stdgserver stdgserver

extern char *pwd;
#define pwd pwd





FILE *open(const char *path,int attr,const char *mode);
int close(FILE *fd);
int flush(FILE *fp);

int creat(const char *path,int attr);
int rmove (const char *path);




int _putc (int ch, FILE *fp);
int _getc (FILE *fp);

int _fputs (const char *str,FILE *fp);
char *_fgets (char *str,int length,FILE *fp);


// 
int exectve(int argc,char **argv);


int set_cursor_x(int x);



#endif

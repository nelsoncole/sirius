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


int creat(const char *path,int attr);
int rmove (const char *path);


// 
int exectve(int argc,char **argv);
int exectve2(int argc,char **argv);


int set_cursor_x(int x);


int getckey();
int setkey();

extern FILE *open_r(const char *path,int attr,const char *mode);
extern int getc_r (FILE *fp);
extern int putc_r (int c,FILE *fp);
extern int fputs_r (const char *str,FILE *fp);
extern char *fgets_r (char *str,int length,FILE *fp);
extern size_t read_r (void *buffer,size_t size, size_t count, FILE *fp);
extern size_t write_r (const void *buffer,size_t size, size_t count, FILE *fp);
extern int flush_r(FILE *fp);
extern int close_r(FILE *fp);


#endif

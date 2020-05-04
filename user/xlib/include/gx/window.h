#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "font.h"

struct ps {
	unsigned int id;
	unsigned int x :12;
	unsigned int y :12;
	unsigned int type :8;
}__attribute__((packed));

typedef struct gx_hand {
	//
	unsigned int flag;
	unsigned int style;
	unsigned int type;
	// font
	struct font font;
	// position x and y
	int x0, y0;
	int x, y;
	// width and height
	int w, h;
	
	void *vmm;
	
	// memory message
	unsigned int m[4];
	//
	struct ps *ps;

	int spin_lock;
	unsigned int pid;

	struct gx_hand *next;
	struct gx_hand *tail;
}__attribute__((packed)) gx_hand_t; 

typedef struct  gx_message {
	unsigned int	id;
	unsigned short 	org;
	unsigned short	type;
}__attribute__((packed)) gx_message_t;


#endif

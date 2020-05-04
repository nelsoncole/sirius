#include <gx.h>
#include <string.h>

void gx_clearscreen(void *bbuffer) 
{
	__setmem(bbuffer,0x0,(G->HorizontalResolution * G->VerticalResolution\
	* (G->Bpp/8)));
}

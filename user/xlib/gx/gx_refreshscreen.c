#include <gx.h>
#include <string.h>

void gx_refreshscreen(void *fbuffer,void *bbuffer) 
{
	__copymem(fbuffer,bbuffer,\
	G->HorizontalResolution * G->VerticalResolution * (G->Bpp/8));
	
}

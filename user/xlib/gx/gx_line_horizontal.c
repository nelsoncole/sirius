#include <gx.h>

int gx_line_horizontal (int x, int y, int w,unsigned int rgba, void *vmm) 
{	
	int i;
	for (i = 0; i < w; i++) {

		gx_put_pixel( vmm, x + i, y, rgba, rgba);
	}
	
	return (w - y);
}

#include <gx.h>

int gx_line_vertical (int x, int y, int h, unsigned int rgba, void *vmm) 
{	
	int i;
	for (i = 0; i < h; i++) {

		gx_put_pixel( vmm, x, y + i, rgba, rgba);
	}
	
	return (h - y);
}

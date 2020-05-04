#include <gx.h>

long gx_rect(long x1, long y1, long x2, long y2,long bg, void *vmm)
{

	int fg = 0;
	long x,y;
	for (y = y1; y < (y1 + y2) ; y++ ) {
		
		for (x = x1; x < (x1 + x2); x++) 
			gx_put_pixel(vmm, x, y, fg, bg);
	}

	return x + y;
}

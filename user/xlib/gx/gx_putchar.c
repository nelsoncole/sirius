#include <gx.h>

char fdata[256];

int gx_putchar(struct font *font,int unicode,long x, long y,long fg, long bg, void *vmm) 
{
	// check sum
	if (font->magic != 1234) return (0);

	font_bitmap(font, fdata, unicode);

    	int cx, cy, v;

   	for(cy=0;cy < font->rows;cy++) {
        	for(cx = 0 ;cx < font->cols;cx++) {
			v = fdata[cy * font->cols + cx] &0xff;
			gx_put_pixel(vmm, x + cx, y + cy, GX_ALPHA(fg, v),bg);
          	}
        
    	}

	return unicode;
}

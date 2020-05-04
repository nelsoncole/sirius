#include <gx.h>

long gx_putpixel(long x, long y, long rgb, void *vmm)
{

	if( x >= G->HorizontalResolution || y >= G->VerticalResolution ) 
		return 0;

	long off = gx_getoffset(x,y);

	(*(unsigned int *)((unsigned int *)vmm + off)) = rgb; 

	return off;
	
}

// RGBA
// creditos: Aquila OS
#define COLORMERGE(f, b, c)	((b) + (((f) - (b)) * (c) >> 8u))
#define _R(c)   (((c) >> 3*8) & 0xFF)
#define _G(c)   (((c) >> 2*8) & 0xFF)
#define _B(c)   (((c) >> 1*8) & 0xFF)
#define _A(c)   (((c) >> 0*8) & 0xFF)

void gx_put_pixel(void *vmm, int x, int y, unsigned int fg, unsigned int bg)
{
	if( x >= G->HorizontalResolution || y >= G->VerticalResolution ) 
		return;

	long off = gx_getoffset(x,y);

    	unsigned char *p = (unsigned char *)((unsigned int *)vmm + off);
	unsigned char *q = (unsigned char *)((unsigned int *)vmm + off);

    	p[0] = COLORMERGE(_B(fg), _B(bg), _A(fg));  // Blue
    	p[1] = COLORMERGE(_G(fg), _G(bg), _A(fg));  // Green
    	p[2] = COLORMERGE(_R(fg), _R(bg), _A(fg));  // Red
	p[3] = _A(fg);  			    // Alpha

	
	int op = 0xf;
    	q[0] = COLORMERGE(p[0], q[0], op);  // Blue
    	q[1] = COLORMERGE(p[1], q[1], op);  // Green
    	q[2] = COLORMERGE(p[2], q[2], op);  // Red
	q[3] = _A(fg);  		    // Alpha
		
}

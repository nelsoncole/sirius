#include <gx.h>


int gx_border (int x1, int y1, int x2, int y2, int len, unsigned int rgba, void *vmm)
{
	// Top
	gx_line_horizontal (x1, y1, x2, rgba, vmm);	
	
	// Left
	gx_line_vertical (x1, y1, y2, rgba, vmm);
	
	// bottom
	gx_line_horizontal (x1, y1 + y2, x2, rgba, vmm);
	
	// right
	gx_line_vertical (x1 + x2, y1, y2 + 1, rgba, vmm);

	return ((x2 - x1) * (y2 - y1));
}

#include <gx.h>

long gx_getoffset(long x, long y)
{
	return ((G->PixelsPerScanLine * y) + x);

}

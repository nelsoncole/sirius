#include <gx.h>


int getmouse_x()
{
	MOUSE *mouse = (MOUSE *)(*(unsigned int*)(0x10001120));

	return (mouse->x);
}

int getmouse_y()
{
	MOUSE *mouse = (MOUSE *)(*(unsigned int*)(0x10001120));

	return (mouse->y);

}

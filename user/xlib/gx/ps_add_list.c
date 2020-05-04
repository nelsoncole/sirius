#include <gx.h>

int ps_add_list(gx_hand_t *window, gx_hand_t *obj)
{
	int i;

	for(i=0;i<512;i++) {
		
		if(window->ps[i].id == 0) {
			window->ps[i].id = (unsigned int) obj;
			window->ps[i].type = obj->type;
			break;
		}
	}

	return (0);
}

#include <gx.h>

int obj_add_list(gx_hand_t *w, gx_hand_t *t) {

	gx_hand_t *tail 	= w;
	while(tail->tail) tail = tail->tail;

	tail->tail = t;	


	return (0);
}

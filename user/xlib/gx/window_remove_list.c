#include <gx.h>

void window_remove_list(gx_hand_t *w) {

	gx_hand_t *next = w->next;

	gx_hand_t *l = (gx_hand_t *) G->l.l; // inicio

	while(G->l.spin_lock[0]);
	while(G->l.spin_lock[1]);
	while(G->l.spin_lock[2]);
	G->l.spin_lock[1] = 1;
	G->l.spin_lock[2] = 1;

	// Percorre a lista ate achar o l->next igual ao w
	do{

		if(l->next == w)break;
		l = l->next;

	}while(l);

	if(!l) { 
		G->l.spin_lock[1] = 0;
		return;
	}

	// aponta o l->next para o next
	l->next = next;

	G->l.spin_lock[1] = 0;
	G->l.spin_lock[2] = 0;

}

void window_remove_list2(gx_hand_t *w) {

	gx_hand_t *next = w->next;

	gx_hand_t *l = (gx_hand_t *) G->l.l2; // inicio

	while(G->l.spin_lock[0]);
	while(G->l.spin_lock[1]);
	while(G->l.spin_lock[2]);
	G->l.spin_lock[1] = 1;
	G->l.spin_lock[2] = 1;

	// Percorre a lista ate achar o l->next igual ao w
	do{

		if(l->next == w)break;
		l = l->next;

	}while(l);

	if(!l) { 
		G->l.spin_lock[1] = 0;
		return;
	}

	// aponta o l->next para o next
	l->next = next;

	G->l.spin_lock[1] = 0;
	G->l.spin_lock[2] = 0;

}

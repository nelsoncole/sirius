#include <gx.h>

int window_add_list(gx_hand_t *w) {
	while(G->l.spin_lock[0]);
	while(G->l.spin_lock[1]);
	while(G->l.spin_lock[2]);
	G->l.spin_lock[1] = 1;
	G->l.spin_lock[2] = 1;

	gx_hand_t *l = (gx_hand_t *) G->l.l;

	while(l->next) l = l->next;

	l->next = w;


	G->l.pid[0] = w->pid;
	G->l.spin_lock[1] = 0;
	G->l.spin_lock[2] = 0;

	return (0);
}


int window_add_list2(gx_hand_t *w) {
	while(G->l.spin_lock[0]);
	while(G->l.spin_lock[1]);
	while(G->l.spin_lock[2]);
	G->l.spin_lock[1] = 1;
	G->l.spin_lock[2] = 1;

	gx_hand_t *l = (gx_hand_t *) G->l.l2;

	while(l->next) l = l->next;

	l->next = w;


	//G->l.pid[0] = w->pid;
	G->l.spin_lock[1] = 0;
	G->l.spin_lock[2] = 0;

	return (0);
}

#include <gx.h>
#define null 0

unsigned int gx_mouse_set_focus(int x, int y) {

	gx_hand_t *w, *q;
	int i,j;

	unsigned int pid = 0;

	if(G->l.spin_lock[0]) return (0);

	while(G->l.spin_lock[2]);
	G->l.spin_lock[2] = 1;

	w = (gx_hand_t *) G->l.l;

	w = w->next;
	if(!w){

		G->l.pid[0] = 0;
		G->l.spin_lock[2] = 0;

		return (pid);
	}

	j = 1;
	while(w->next) { 
		w = w->next;
		j++;
	}
	// comparar limites
	for(i = 0;i < j;i++) {

		if ((w->x + w->w) >= x && w->x <= x && (w->y + w->h) >= y && w->y <= y) {

			if(G->l.pid[0] == w->pid) {
				G->l.spin_lock[2] = 0;
				return (0);
			};

			pid = w->pid;
			G->l.pid[0] = pid;

			q = (gx_hand_t *) G->l.l;
			while(q) { 
				if(q->next == w ) break;
				q = q->next;			
			}

			while(G->l.spin_lock[1]);
			G->l.spin_lock[1] = 1;

			q->next = w->next;
			w->next = null;

			// add list
			q = (gx_hand_t *) G->l.l;
			while(q->next) q = q->next;
			q->next = w;

			G->l.spin_lock[1] = 0;
			G->l.spin_lock[2] = 0;

			return (pid);
		}

		// anterior
		q = (gx_hand_t *) G->l.l;
		while(q) { 
			if(q->next == w ) {	
				w = q;
				break;			
			}
			q = q->next;			
		}
	}

	G->l.pid[0] = pid;

	G->l.spin_lock[2] = 0;


	return (pid);
}

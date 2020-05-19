#include <gx.h>

gx_hand_t *mx = 0;
int magic = 0;
MOUSE *_mouse_r = 0;

unsigned int rcolor[4];
int fl;

void gx_mouse_init()
{
	_mouse_r = (MOUSE*)(*(unsigned int*)(0x10001120));

	magic = 1234;
	mx = 0;
	fl = 0;
}

void restore () {
	if(!mx) return;
	mx->c[0] = rcolor[0];
	mx->c[1] = rcolor[1];
	mx->c[2] = rcolor[2];
	mx->c[3] = rcolor[3];
	mx->f = 0;
	mx = 0;
	fl = 0;
}

gx_message_t gx_mouse (gx_hand_t *w) {

	gx_message_t r;

	int x = _mouse_r->x;
	int y = _mouse_r->y;
	
	r.id = 0;
	r.org = 1;
	r.type = 0;

	if(magic != 1234) return (r);
	if(G->l.pid[0] != w->pid) return (r);

	int i;
	gx_hand_t *h;
	if(!w->ps) return (r);

	for (i = 0;i < 512;i++) 
	{
		if (!w->ps[i].id) break;
		
		h = (gx_hand_t *) w->ps[i].id;

		if ((h->x + h->w) >= x && h->x <= x && (h->y + h->h) >= y && h->y <= y) 
		{

			if(mx != h) { 
				restore ();
				mx = (gx_hand_t *) w->ps[i].id;
			}

			switch (w->ps[i].type) {

				case GX_TYPE_TITLE:
					if(!fl) {
						rcolor[0] = h->c[0];
						rcolor[1] = h->c[1];
						rcolor[2] = h->c[2];
						rcolor[3] = h->c[3];
						fl = 1;

					}

					
					if((h->x + 8+8) >= x && (h->x + 8) <= x) {
						h->c[2] = 0xff000000;
						
						if(_mouse_r->b&0x1) {
							while(_mouse_r->b&0x1); // espera soltar
							r.type = GX_EXIT;
						}
						
					}else {

						// movimentar janela
						int x0 = x - w->x;
						int y0 = y - w->y;

						while(_mouse_r->b&0x1) 	{

							w->x = _mouse_r->x - x0;
							w->y = _mouse_r->y - y0;
						}					
						restore ();
						return (r);

					}
				break;
				case GX_TYPE_EDITBOX:
					if(!fl) {
						rcolor[0] = h->c[0];
						rcolor[1] = h->c[1];
						rcolor[2] = h->c[2];
						rcolor[3] = h->c[3];
						fl = 1;

					}
					if(!h->f && _mouse_r->b&0x1) {
						while(_mouse_r->b&0x1);
						setkey();
						h->f = 1;
					}
					motor_editbox(h);
				break;
				case GX_TYPE_BUTTON:
					if(!fl) {
						rcolor[0] = h->c[0];
						rcolor[1] = h->c[1];
						rcolor[2] = h->c[2];
						rcolor[3] = h->c[3];
						fl = 1;

					}
					
					h->c[1] = 0xff0000;

					if(_mouse_r->b&0x1) {
						h->f = 1;
						while(_mouse_r->b&0x1) ;
						r.type = GX_OK;
						

					}else h->f = 0;
				break;
				default:
					// restore
					restore ();
				break;
			}



			switch (r.type) {
				case GX_EXIT:
					exit_window(w);
					exit(0);
				break;

			}
		
			r.id = w->ps[i].id;
			return (r);
		}
		
		
	}

	// default
	restore ();
	return (r);
} 

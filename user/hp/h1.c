#include <gx.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SPACE 8

gx_hand_t *L1,*L2,*L3,*L4,*L5,*L6,*L7;
gx_hand_t *NAME,*ID,*NASC,*SEXO,*BI,*TL,*EMAIL,*SALVE;
FILE *f;

static int cmd_salve();

int main(int argc, char **argv)
{

	gx_message_t m;
	gx_mouse_init();

	gx_hand_t *window = create_window (0,0, 640, 300, 0);
	create_title(window, "Novo Paciente", 1, 0);

	L1 = create_label(window,"Nome",8,32,window->font.cols*4,window->font.rows,0);
	NAME = create_editbox(window,L1->x,L1->y + L1->h + SPACE,window->font.cols*50,window->font.rows+4,64, 0/*normal*/);

	L2 = create_label(window,"P-ID",NAME->x + NAME->w + SPACE,32,window->font.cols*4,window->font.rows,0);
	ID = create_editbox(window,L2->x,L2->y + L2->h + SPACE,window->font.cols*16,window->font.rows+4,64, 0/*normal*/);
	
	L3 = create_label(window,"Data de Nasc.",8,NAME->y + NAME->h + SPACE,window->font.cols*13,window->font.rows,0);
	NASC = create_editbox(window,L3->x,L3->y + L3->h + SPACE,window->font.cols*20,window->font.rows+4,64, 0/*normal*/);

	L4 = create_label(window,"Sexo",NASC->x + NASC->w + SPACE,NASC->y,window->font.cols*4,window->font.rows,0);
	SEXO = create_editbox(window,L4->x + L4->w + SPACE,NASC->y,window->font.cols*20,window->font.rows+4,64, 0/*normal*/);


	L5 = create_label(window,"BI",SEXO->x + SEXO->w + SPACE,NASC->y,window->font.cols*2,window->font.rows,0);
	BI = create_editbox(window,L5->x + L5->w + SPACE,NASC->y,window->font.cols*20,window->font.rows+4,64, 0/*normal*/);


	L6 = create_label(window,"Telefone",8,NASC->y + NASC->h + SPACE,window->font.cols*13,window->font.rows,0);
	TL = create_editbox(window,L6->x,L6->y + L6->h + SPACE,window->font.cols*20,window->font.rows+4,64, 0/*normal*/);

	L7 = create_label(window,"Email",TL->x + TL->w + SPACE,L6->y,window->font.cols*5,window->font.rows,0);
	EMAIL = create_editbox(window,L7->x,L7->y + L7->h + SPACE,window->font.cols*40,window->font.rows+4,64, 0/*normal*/);

	SALVE = create_button(window, "Salvar",window->w - (window->font.cols*8 + SPACE) ,
	window->h - (window->font.rows+4+SPACE), window->font.cols*8, window->font.rows+4, 0);



	for(;;) {	

		m = gx_mouse (window);

		switch(m.type) {
			case GX_OK:
				cmd_salve();
				//exit_window(window);
				//exit(0);
			break;


		}


	}
	return 0;
}


static int cmd_salve() 
{
	int rc = 0;

	char *fname = (char*) get_buffer_editbox(ID);
	if(!*fname) return -1;

	f = fopen(fname,"w");
	if( f == NULL) return -1;

	// NAME 64 
	char *buf = (char*) get_buffer_editbox(NAME);
	int len = get_len_editbox(NAME);
	if( fwrite(buf,1,len,f) != len ) return -1;

	// ID 8
	buf = (char*) get_buffer_editbox(ID);
	if( fwrite(buf,1,64,f) != 64 ) return -1;

	// NASC 4
	buf = (char*) get_buffer_editbox(NASC);
	if( fwrite(buf,1,64,f) != 64 ) return -1;

	// SEXO 4
	buf = (char*) get_buffer_editbox(SEXO);
	if( fwrite(buf,1,64,f) != 64 ) return -1;

	// BI 16
	buf = (char*) get_buffer_editbox(BI);
	if( fwrite(buf,1,64,f) != 64 ) return -1;

	// TL 4
	buf = (char*) get_buffer_editbox(TL);
	if( fwrite(buf,1,64,f) != 64 ) return -1;

	// EMAIL 64
	buf = (char*) get_buffer_editbox(EMAIL);
	len = get_len_editbox(EMAIL);
	if( fwrite(buf,1,len,f) != len ) return -1;

	fclose(f);

	return rc;
}

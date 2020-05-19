#include <gx.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SPACE 8

gx_hand_t *B0,*B1,*B2,*B3,*B4,*B5,*B6,*B7,*B8,*B9,*DIV,*MUL,*SUB,*VIR,*PER,*ADD,*IGUAL,*BC;
gx_hand_t *MONITOR,*BN;

char *a, *b, *c;
int sinal = 0;

int main(int argc, char **argv)
{

	a = (char *) malloc(128);
	b = (char *) malloc(128);
	c = (char *) malloc(128);

	memset(a,0,128);
	memset(b,0,128);
	memset(c,0,128);


	gx_message_t m;
	gx_mouse_init();

	gx_hand_t *window = create_window (0,0, 240, 300, 0);
	create_title(window, "Calculadora", 1, 0);


	BC = create_button(window, "C", SPACE,150, window->font.cols*6, window->font.rows+4, 0);
	BN = create_button(window, " ", BC->x + BC->w + SPACE,BC->y, window->font.cols*6, window->font.rows+4, 0);
	BN = create_button(window, " ", BN->x + BN->w + SPACE,BC->y, window->font.cols*6, window->font.rows+4, 0);
	DIV = create_button(window, "÷", BN->x + BN->w + SPACE,BC->y, window->font.cols*6, window->font.rows+4, 0);


	B7 = create_button(window, "7", SPACE,BC->y + BC->h + SPACE, window->font.cols*6, window->font.rows+4, 0);
	B8 = create_button(window, "8", B7->x + B7->w + SPACE,B7->y, window->font.cols*6, window->font.rows+4, 0);
	B9 = create_button(window, "9", B8->x + B8->w + SPACE,B7->y, window->font.cols*6, window->font.rows+4, 0);
	MUL = create_button(window, "×", B9->x + B9->w + SPACE,B7->y, window->font.cols*6, window->font.rows+4, 0);

	B4 = create_button(window, "4", SPACE,B7->y + B7->h + SPACE, window->font.cols*6, window->font.rows+4, 0);
	B5 = create_button(window, "5", B4->x + B4->w + SPACE,B4->y, window->font.cols*6, window->font.rows+4, 0);
	B6 = create_button(window, "6", B5->x + B5->w + SPACE,B4->y, window->font.cols*6, window->font.rows+4, 0);
	SUB = create_button(window, "-", B6->x + B6->w + SPACE,B4->y, window->font.cols*6, window->font.rows+4, 0);

	B1 = create_button(window, "1", SPACE,B4->y + B4->h + SPACE, window->font.cols*6, window->font.rows+4, 0);
	B2 = create_button(window, "2", B1->x + B1->w + SPACE,B1->y, window->font.cols*6, window->font.rows+4, 0);
	B3 = create_button(window, "3", B2->x + B2->w + SPACE,B1->y, window->font.cols*6, window->font.rows+4, 0);
	ADD = create_button(window, "+", B3->x + B3->w + SPACE,B1->y, window->font.cols*6, window->font.rows+4, 0);

	B0 = create_button(window, "0", SPACE,B1->y + B1->h + SPACE, window->font.cols*6, window->font.rows+4, 0);
	VIR = create_button(window, ",", B0->x + B0->w + SPACE,B0->y, window->font.cols*6, window->font.rows+4, 0);
	PER = create_button(window, "%", VIR->x + VIR->w + SPACE,B0->y, window->font.cols*6, window->font.rows+4, 0);
	IGUAL = create_button(window, "=", PER->x + PER->w + SPACE,B0->y, window->font.cols*6, window->font.rows+4, 0);


	MONITOR = create_editbox(window, SPACE,32,IGUAL->x + IGUAL->w -SPACE,100,256, 0/*normal*/);


	gx_hand_t *cmp;

	int i = 0;
	char *r = a; 

	long d;

	for(;;) {	

		m = gx_mouse (window);

		switch(m.type) {
			case GX_OK:

				cmp = (gx_hand_t *)m.id; 
				if(cmp == B9 ) {
					r_motor_editbox(MONITOR,'9');
					r[i++] = '9';
				} else if (cmp == B8) {
					r_motor_editbox(MONITOR,'8');
					r[i++] = '8';
					
				} else if (cmp == B7) {
					r_motor_editbox(MONITOR,'7');
					r[i++] = '7';
				} else if (cmp == B6) {
					r_motor_editbox(MONITOR,'6');
					r[i++] = '6';
				} else if (cmp == B5) {
					r_motor_editbox(MONITOR,'5');
					r[i++] = '5';
				} else if (cmp == B4) {
					r_motor_editbox(MONITOR,'4');
					r[i++] = '4';
				} else if (cmp == B3) {
					r_motor_editbox(MONITOR,'3');
					r[i++] = '3';
				} else if (cmp == B2) {
					r_motor_editbox(MONITOR,'2');
					r[i++] = '2';
				} else if (cmp == B1) {
					r_motor_editbox(MONITOR,'1');
					r[i++] = '1';
				} else if (cmp == B0) {
					r_motor_editbox(MONITOR,'0');
					r[i++] = '0';
				} else if (cmp == DIV) {
					if(*r) {
						r_motor_editbox(MONITOR,':');
						i = 0;
						r = b;
						sinal = 1;
					}
				} else if (cmp == MUL) {
					if(*r) {
						r_motor_editbox(MONITOR,'*');
						i = 0;
						r = b;
						sinal = 2;
					}
				} else if (cmp == SUB) {
					if(*r) {
						r_motor_editbox(MONITOR,'-');
						i = 0;
						r = b;
						sinal = 3;
					} else {

						r_motor_editbox(MONITOR,'-');
						r[i++] = '-';

					}
				} else if (cmp == ADD) {
					if(*r) {
						r_motor_editbox(MONITOR,'+');
						i = 0;
						r = b;
						sinal = 4;
					}
				} else if (cmp == IGUAL) {

					i = 0;
					switch(sinal) {
						case 1:
							d = atol(a) / atol(b);
							itoa (d,c);
						break;
						case 2:
							d = atol(a) * atol(b);
							itoa (d,c);
						break;
						case 3:
							d = atol(a) - atol(b);
							itoa (d,c);
						break;
						case 4:
							d = atol(a) + atol(b);
							itoa (d,c);
						break;

					}

					r_motor_editbox(MONITOR,'\n');
					r = c;
					while(*r)r_motor_editbox(MONITOR,*r++);

					sinal = 0;
					r = a;

					memset(a,0,128);
					memset(b,0,128);
					memset(c,0,128);
					
				} else if (cmp == BC) {
					clrs_editbox(MONITOR);
					sinal = 0;
					r = a;
					i = 0;

					memset(a,0,128);
					memset(b,0,128);
					memset(c,0,128);
				}
			break;


		}


	}
	return 0;
}

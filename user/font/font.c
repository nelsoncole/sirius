#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>


typedef struct tinyfont {
	char sig[8];	// tinyfont signature; "tinyfont"
	int ver;	// version; 0
	int n;		// number of glyphs
	int rows;
	int cols;
}__attribute__((packed)) tinyfont_t;


int main(int argc, char **argv) {

	//if(argc < 2) { fprintf(stdout,"error: not argument"); return (-1); }

	FILE *file = fopen("ubuntu.ttf"/*argv[1]*/,"r");
	if(!file) { fprintf(stdout,"error: file not found"); return (-1); }

	tinyfont_t *ttf = (tinyfont_t *) malloc(0x10000);
	
	int r = fread(ttf,sizeof(tinyfont_t),1,file);

	r = r;

	fprintf(stdout,"tinyfont signature: %x %x\n",*(unsigned*)ttf->sig,*(unsigned*)ttf->sig+4);
	fprintf(stdout,"version: 0x%x\n",ttf->ver);
	fprintf(stdout,"number of glyphs: %x\n",ttf->n);
	fprintf(stdout,"rows: %x\n",ttf->rows);
	fprintf(stdout,"cols: %x\n",ttf->cols);


			/*char a[4];
			char rgb[4];

			int off = 0;
			for(int y=0;y < 16;y++) {

				for(int x=0;x < 8;x++) {

					*(unsigned int*)a =  (*(unsigned int*)((ttf+24) + (3*off++))) &0xffffff;

					rgb[0] = a[0];
					rgb[1] = a[1];
					rgb[2] = a[2];
					rgb[3] = 0xff;
					PutPixelBuff(x+200,y+100,*(unsigned int*)rgb,G->BankBuffer);

				}
			}


			app_refreshrate();*/



	free(ttf);
	return (0);

}

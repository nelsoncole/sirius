#include <gx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
	Creditos: Aquila OS 

*/

static int find_glyph(struct font *font, int c)
{
	int l = 0;
	int h = font->n;
	while (l < h) {
		int m = (l + h) / 2;
		if (font->glyphs[m] == c)
			return m;
		if (c < font->glyphs[m])
			h = m;
		else
			l = m + 1;
	}
	return -1;
}

int font_bitmap(struct font *font, void *dst, int c)
{
	int i = find_glyph(font, c);
	int len = font->rows * font->cols;
	if (i < 0)
		return 1;
	memcpy(dst, font->data + i * len, len);
	return 0;
}


int gx_font_init (struct font *font, char *path, int type) {

	struct tinyfont *tf = 0;

	font->magic = 0;
	int r = 0;
	FILE *f = fopen(path, "r");
	if(!f) {
		return (-1);
	}


	switch (type) {
		case 1:
			tf = (struct tinyfont *) malloc(sizeof(struct tinyfont));
			
			if ( fread(tf,sizeof(struct tinyfont),1,f) != 1) 
			{
				free(tf);
				fclose(f);
				return (1);
			}

			// check signature: "tinyfont"
			if( strncmp(tf->sig,"tinyfont",8) ) {
				free(tf);
				fclose(f);
				return (1);
			}

			font->rows = tf->rows; 
			font->cols = tf->cols;
			font->n = tf->n;
			font->glyphs = (unsigned int *) malloc(tf->n * sizeof(unsigned int));
			font->data = (char *) malloc(font->n * font->rows * font->cols);

			if (fread(font->glyphs,font->n * sizeof(int),1,f) != 1) 
			{
				free(font->glyphs);
				free(font->data);
				free(tf);
				fclose(f);
				return (-1);
			}
			if (fread(font->data,font->n * font->rows * font->cols,1,f) != 1) 
			{
				free(font->glyphs);
				free(font->data);
				free(tf);
				fclose(f);
				return (-1);
			}
		

			r = 0;
			free(tf);
			
		break;
		default:
			r = 1;
		break;
	}


	font->magic = 1234;
	fclose(f);
	return (r);
}

void gx_font_end(struct font *font)
{	font->magic = 0;
	free(font->glyphs);
	free(font->data);
}

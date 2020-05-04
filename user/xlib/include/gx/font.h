#ifndef __FONT_H__
#define __FONT_H__


struct font {
	// magic
	int magic;
	// glyph bitmap rows and columns
	int	rows; 
	int	cols;
	// number of font glyphs	
	int 	n;
	// glyph unicode character codes
	unsigned int *glyphs;
	// glyph bitmaps
	char 	*data;
}__attribute__((packed));


/*struct ttf{
		uint32_t tableVersion;
		uint16_t fontRevision;
		uint32_t checkSumAdjustment;
		uint32_t magicNumber;//0x5F0F3CF5
		uint16_t flags;
		uint16_t unitsPerEm;
		long long createdDate;
		long long modifiedData;
		short xMin;
		short yMin;
		short xMax;
		short yMax;
		uint16_t macStyle;
		uint16_t lowestRecPPEM;
		short fontDirectionHintl;
		short indexToLocFormat;
		short glyphDataFormat;

}__attribute__((packed));*/

struct tinyfont {
	char 		sig[8];	/* tinyfont signature; "tinyfont" */
	unsigned int 	ver;	/* version; 0 */
	int n;			/* number of glyphs */
	int rows, cols;	/* glyph dimensions */
}__attribute__((packed));


int font_bitmap(struct font *font, void *dst, int c);
int gx_font_init (struct font *font, char *path, int type);
void gx_font_end(struct font *font);


#endif

#ifndef __GX_H__
#define __GX_H__

#include <io.h>
#include <gx/font.h>
#include <gx/window.h>
#include <gx/bmp.h>

#define GX_TYPE_WINDOW 	1
#define GX_TYPE_TITLE 	2
#define GX_TYPE_BOX 	3
#define GX_TYPE_LABEL	4
#define GX_TYPE_EDITBOX	5
#define GX_TYPE_LISTBOX	6
#define GX_TYPE_BUTTON	7

#define	GX_EXIT		0x10
#define GX_OK		0x11

#define GX_ALPHA(c, a) (((c) & (~0xFF)) | ((a) & 0xFF))
#define GX_RGBA(r, g, b, a) (((r) << 3*8) | ((g) << 2*8) | ((b) << 1*8) | ((a) << 0*8))
#define GX_RGB(r, g, b) (((r) << 3*8) | ((g) << 2*8) | ((b) << 1*8))

extern void gx_refreshscreen(void *fbuffer,void *bbuffer);
extern void gx_clearscreen(void *bbuffer);
extern long gx_getoffset(long x, long y);
extern long gx_putpixel(long x, long y, long rgb, void *vmm);
extern long gx_rect(long x1, long y1, long x2, long y2,long bg, void *vmm);

extern int gx_putchar(struct font *font,int unicode,long x, long y,long fg, long bg, void *vmm);

extern void gx_put_pixel(void *vmm, int x, int y, unsigned int fg, unsigned int bg);

extern int gx_line_horizontal (int x, int y, int w,unsigned int rgba, void *vmm);
extern int gx_line_vertical (int x, int y, int h,unsigned int rgba, void *vmm);
extern int gx_border (int x1, int y1, int x2, int y2, int len, unsigned int rgba, void *vmm);

extern int window_add_list(gx_hand_t *w);
extern int window_add_list2(gx_hand_t *w);
extern int obj_add_list(gx_hand_t *w, gx_hand_t *t);
extern void window_remove_list(gx_hand_t *w);
extern void window_remove_list2(gx_hand_t *w);
extern void obj_remove_list(gx_hand_t *w, gx_hand_t *t);

extern gx_hand_t *create_window (int x, int y, int w, int h, unsigned int style);
extern gx_hand_t *create2_window (int x, int y, int w, int h, unsigned int style);
extern void exit_window(gx_hand_t *w);
extern void exit2_window(gx_hand_t *w);
extern int update_window(gx_hand_t *w);

extern gx_hand_t *create_title(gx_hand_t *window, const char *title, int x, unsigned int style);
extern void exit_title(gx_hand_t *t);
extern int update_title(gx_hand_t *window, gx_hand_t *t);

extern gx_hand_t *create_box(gx_hand_t *window,int x, int y, int w, int h, unsigned int style);
extern void *get_buffer_box(gx_hand_t *box);
extern void clearscreen_box(gx_hand_t *box);
extern void exit_box(gx_hand_t *box);
extern int update_box(gx_hand_t *window, gx_hand_t *box);

extern gx_hand_t *create_label(gx_hand_t *window, const char *str,int x, int y, int w, int h, unsigned int style);
extern void exit_label(gx_hand_t *l);
extern int update_label(gx_hand_t *window, gx_hand_t *l);
extern int update_str_label(gx_hand_t *l, const char *str);

extern void gx_mouse_init();
extern gx_message_t gx_mouse (gx_hand_t *w);

extern int getmouse_x();
extern int getmouse_y();

extern unsigned int gx_mouse_set_focus(int x, int y);


extern int ps_add_list(gx_hand_t *window, gx_hand_t *obj);


extern void DrawMouse(	int x, int y,	
		unsigned int FgColor, 
		void *Buffer,
		void *MouseBuffer);

extern unsigned short gx_cursor18x18[18];

extern int BitMAP( void *Data, int X, int Y, void *BankBuffer);


#endif

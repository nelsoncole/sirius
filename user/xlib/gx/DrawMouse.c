#include <gx.h>

void DrawMouse(	int x, int y,	
		unsigned int FgColor, 
		void *Buffer,
		void *MouseBuffer)
{
	unsigned short font = 0;
	const unsigned short *font_data = (const unsigned short *) MouseBuffer;
   	int cx, cy;
	unsigned short mask;
    
   	for(cy=0;cy < 18;cy++) {
		mask = 1;
        	font = font_data[cy];
		
                for(cx = 18 -1 ;cx >= 0;cx--){ 
                       if(font &mask)
			gx_put_pixel(Buffer, x + cx, y + cy, FgColor << 8, FgColor << 8);
			mask += mask;
                  
          	}
        
    	}

}

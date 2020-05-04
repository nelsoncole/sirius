#include <mm.h>
#include <stdlib.h>
#include <string.h>

void _free_r(void *ptr,_mm_t *mm)
{
	_mm_t *_mm = mm;
	int i;
	for(i=0;i<_MM_R_SIZE;i++) {

		if ( _mm->mm == (long)ptr ) 
		{
			if( _mm->act == (-1))
			{
				if(mm[_mm->id].act == 1)
				{
					__asm__ __volatile__("int $0x72"::"a"(3),"d"(mm[_mm->id].mm));

					memset((void *)&mm[_mm->id],0,sizeof(_mm_t));
				
				} else mm[_mm->id].act -= 1;
				
				memset(_mm,0,sizeof(_mm_t));

			}else if (_mm->act == 1) {
				
				__asm__ __volatile__("int $0x72"::"a"(3),"d"(ptr));
				memset(_mm,0,sizeof(_mm_t));
			}else _mm->act--;

			return;

		} else _mm++;
	}

}

#undef        free

void free(void *ptr) {

	if(!ptr) return;
	_free_r(ptr,_mm_r);
}

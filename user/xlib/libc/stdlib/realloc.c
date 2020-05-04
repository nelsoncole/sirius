#include <mm.h>
#include <stdlib.h>
#include <string.h>

void *_realloc_r(void *ptr,size_t len,_mm_t *mm) 
{
	int i;
	_mm_t *_mm = mm;
	char *nmm;
	for(i=0;i<_MM_R_SIZE;i++) {

		if ( _mm->mm == (long)ptr ) 
		{
			nmm = (char*) malloc(len);

			memcpy(nmm,ptr,_mm->len);
			free(ptr);

			return (void*) nmm;

		} else _mm++;
	}

	return (void*) malloc(len);

}

void *realloc(void *ptr, size_t size)
{
	if(!ptr) return (void*) malloc(size);

	return (void*)_realloc_r(ptr,size,_mm_r);
}

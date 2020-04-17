#include <mm.h>
#include <stdlib.h>
#include <string.h>

/*
typedef struct _mm_ {
	long mm;
	long id;
	long act;	
	long len; 

}__attribute__((packed)) _mm_t;*/

void *_realloc_r(void *ptr,size_t len,_mm_t *mm) 
{
	int i;
	_mm_t *_mm = mm;
	char *nmm;
	for(i=0;i<8192;i++) {

		if ( _mm->mm == (long)ptr ) 
		{
			nmm = (char*) malloc(len);

			memcpy(nmm,ptr,_mm->len);
			free(ptr);

			return nmm;

		} else _mm++;
	}

	return 0;

}

void *realloc(void *ptr, size_t size)
{
	if(!ptr) return malloc(size);

	return _realloc_r(ptr,size,_mm_r);
}

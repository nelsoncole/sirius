#include <mm.h>
#include <stdlib.h>


/*
typedef struct _mm_ {
	long mm;
	long id;
	long act;	
	long len; 

}__attribute__((packed)) _mm_t;*/


_mm_t _mm_r[8192]; // 96 KiB


_mm_t *_malloc_r(_mm_t *mm, size_t len) 
{
	int i;
	_mm_t *_mm = mm;
	for(i=0;i<8192;i++) {

		if ( _mm->mm == 0 ) 
		{
			_mm->id = i;
			_mm->act++;
			_mm->len = len;
			__asm__ __volatile__("\
			int $0x72":"=a"(_mm->mm):"a"(2),"d"(len));

			return _mm;

		} else _mm++;
	}

	return 0;

}


_mm_t *_malloc_min_r(_mm_t *mm, size_t len) 
{
	if(len > 4096) return 0;

	int i;
	_mm_t *_mm = mm;

	long add = 0;
	long id = 0;

	for(i=0;i<8192;i++) {

		if ( _mm->mm != 0 && (_mm->len+len <= 4096) && _mm->act != (-1) ) 
		{
			_mm->act++;
			add = _mm->mm + _mm->len;
			id = _mm->id;
			_mm->len += len;
			break;

		} else _mm++;
	}

	if(i >= 8192) return 0;

	_mm = mm;
	for(i=0;i<8192;i++) {

		if ( _mm->mm == 0 ) 
		{
			_mm->act = (-1);
			_mm->id = id;
			_mm->len = len;
			_mm->mm = add;
			return _mm;

		} else _mm++;
	}

	return 0;

}




#undef        malloc

void *malloc(size_t size) {

	_mm_t *mm = _malloc_min_r(_mm_r,size);

	if(!mm) mm = _malloc_r(_mm_r,size);

	if(!mm) return (void*)0;
	
	return (void*)(mm->mm);

}

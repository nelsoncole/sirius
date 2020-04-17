#ifndef __MM_H__
#define __MM_H__

typedef struct _mm_ {
	long mm;
	long id;
	long act;
	long len; 

}__attribute__((packed)) _mm_t;


extern _mm_t _mm_r[8192]; // 96 KiB

#endif

#ifndef __MM_H__
#define __MM_H__

#define _MM_R_SIZE 	1024
#define _MM_BLOCK_SIZE 	4096

typedef struct _mm_ {
	unsigned int mm;
	unsigned int id;
	unsigned int act;
	unsigned int len; 

}__attribute__((packed)) _mm_t;


extern _mm_t _mm_r[_MM_R_SIZE];

#endif

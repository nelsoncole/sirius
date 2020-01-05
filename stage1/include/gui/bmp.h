/**
 *	Copyright (c) 2013 Sirius Corporation
 *	Nelson Sapalo da Siva Cole
 *	Lubango, 26 de Novembro de 2019 - Hora: 20h47
 *	File Name: bmp.h
 **/


#ifndef __BMP_H__
#define __BMP_H__


typedef struct _BMP_FILE {
	UINT16 	type;
	UINT32 	size;
	UINT16	rsv1;
	UINT16	rsv2;
	UINT32	offset_bits;
}__attribute__((packed)) BMP_FILE;

typedef struct _BMP_INFO {
	UINT32 	size;
	UINT32 	width;
	UINT32	height;
	UINT16	planes;
	UINT16	count;
	UINT32	compress;
	UINT32	size_imag;
	UINT32	hres;
	UINT32	vres;
	UINT32	clr_used;
	UINT32	clr_impor;
	
	
}__attribute__((packed)) BMP_INFO;

typedef struct _BMP_TABLE_COLOR {
	UINT8 	blue;
	UINT8 	green;
	UINT8	red;
	UINT8	rsv;
	
}__attribute__((packed)) BMP_TABLE_COLOR;

typedef struct _BMP {
	BMP_FILE bformat;
	BMP_INFO binfo;
}__attribute__((packed)) BMP;


UINTN BitMAP(	VOID *Data,	
		UINTN X,
		UINTN Y,
		VOID *BankBuffer);



#endif

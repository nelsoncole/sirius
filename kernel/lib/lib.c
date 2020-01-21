/*
 * File Name: lib.c
 *
 *
 * BSD 3-Clause License
 * 
 * Copyright (c) 2019, nelsoncole
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 
#include <os.h>

/*
 *  Modo 	Significado
 *
 * r		Abre um arquivo-texto para leitura
 * w		Cria um arquivo-texto para escrita
 * a		Anexa a um arquivo-texto
 * rb		Abre um arquivo binário para leitura
 * wb		Cria um arquivo binário para escrita
 * ab		Anexa a um arquivo binário
 * r+		Abre um arquivo-texto para leitura/escrita
 * w+		Cria um arquivo-texto para escrita/escrita
 * a+		Anexa ou cria um arquivo-texto para escrita/escrita
 * r+b		Abre um arquivo binário para leitura/escrita
 * w+b		Cria um arquivo binário para escrita/escrita
 * a+b		Anexa ou cria um arquivo binário para escrita/escrita
 *
 *	
 */

FILE *open (const char *filename,const char *mode) 
{
	FILE *stream 	= (FILE *)malloc(0x1000);
	FAT_DATA *data 	= (FAT_DATA*)malloc(sizeof(FAT_DATA));
	MBR *mbr	= (MBR*)malloc(0x200);
	VOLUME *volume	= (VOLUME*)malloc(sizeof(VOLUME));

	setmem(stream,0x1000,0);

	if(read_sector(DEV,1,0,mbr) ) goto error;

	volume->lba_start = mbr->part[0].lba_start;
	volume->dev_num = DEV;



	//FAT TYPE
	FAT_BPB  *bpb  	= FatReadBPB(volume);

	if(bpb == NULL) goto error;

	FAT_DIRECTORY *root =FatOpenRoot(bpb,data);

	if(root == NULL) { free(bpb);  goto error; }

	if( FatOpenFile(bpb,data,root,filename,ATTR_ARCHIVE,stream) ) {

		if((*mode == 'w') || (*mode == 'a') ) {

			if( FatCreateFile(bpb,data,root,filename,ATTR_ARCHIVE) ) { 
				free(bpb); free(root); goto error; 
			}else {
				FAT_DIRECTORY *_root = FatUpdateRoot(bpb,data,root);
				if(_root == NULL) { 
					free(bpb); free(root); goto error; 
				}

			}
			if( FatOpenFile(bpb,data,root,filename,ATTR_ARCHIVE,stream) ) { 
				free(bpb); free(root); goto error; 
			}

		} else { free(bpb); free(root); goto error; }
	}

	free(volume);
	free(mbr);
	free(bpb);
	free(root);
	free(data);

	stream->header.offset_lo = 0;
	stream->header.offset_hi = 0;
	stream->header.current 	= NULL;
	stream->header.next 	= NULL;
	stream->header.buffer = (unsigned int)malloc(0x1000);
	return stream;

error:
	free(volume);
	free(mbr);
	free(data);
	free(stream);
	return NULL;
}

int close (FILE *stream) 
{
	free((void*)(stream->header.buffer));
	free(stream);
	return EOF; // successfull
}

int putc (int ch, FILE *stream) 
{
	unsigned char* buffer = (unsigned char*) (stream->header.buffer);
	unsigned dev_n = stream->header.dev;
	unsigned count = stream->header.count;
	unsigned int offset_lo = stream->header.offset_lo;
	unsigned int offset_hi = stream->header.offset_hi;
	unsigned int total_blocks = stream->header.blocks;
	unsigned int *block	= (unsigned int*)(stream->block);
	unsigned int bps = stream->header.bps;
	
	if(!total_blocks) return EOF;

	if(read_sector(dev_n,1,block[offset_lo/bps],buffer)) return EOF;

	// write character
	*(unsigned char*)(buffer + offset_lo) = ch;

	// gravar no disco
	if(write_sector(dev_n,1,block[offset_lo/bps],buffer)) return EOF;
	
	stream->header.size += 1;
	if(stream->header.offset_lo < 0xFFFFFFFF)stream->header.offset_lo +=1;
	else stream->header.offset_hi +=1;

	return  (ch&0xff); // successfull

	// unused
	return (offset_hi + count);
}

int getc (FILE *stream)
{
	unsigned char* buffer = (unsigned char*) (stream->header.buffer);
	unsigned dev_n = stream->header.dev;
	unsigned count = stream->header.count;
	unsigned int offset_lo = stream->header.offset_lo;
	unsigned int offset_hi = stream->header.offset_hi;
	unsigned int total_blocks = stream->header.blocks;
	unsigned int *block	= (unsigned int*)(stream->block);
	unsigned int bps = stream->header.bps;
	
	if(!total_blocks) return -1;

	if(read_sector(dev_n,1,block[offset_lo/bps],buffer)) return 1;

	if(stream->header.offset_lo < 0xFFFFFFFF)stream->header.offset_lo +=1;
	else stream->header.offset_hi +=1;

	return *(unsigned char*)(buffer + offset_lo); // successfull

	// unused
	return (offset_hi + count);

}

int flush(FILE *stream) {

	FAT_DATA *data 	= (FAT_DATA*)malloc(sizeof(FAT_DATA));
	MBR *mbr	= (MBR*)malloc(0x200);
	VOLUME *volume	= (VOLUME*)malloc(sizeof(VOLUME));

	if(read_sector(DEV,1,0,mbr) ) goto error;

	volume->lba_start = mbr->part[0].lba_start;
	volume->dev_num = DEV;



	//FAT TYPE
	FAT_BPB  *bpb  	= FatReadBPB(volume);

	if(bpb == NULL) goto error;

	FAT_DIRECTORY *root =FatOpenRoot(bpb,data);

	if(root == NULL) { free(bpb);  goto error; }

	if(FatUpdateFile(bpb,data,root,stream)) {
		free(bpb); free(root); goto error;
	}

	free(volume);
	free(mbr);
	free(bpb);
	free(root);
	free(data);
	return 0;

error:
	free(volume);
	free(mbr);
	free(data);
	return -1;

}

int eof (FILE *stream)
{

	return 0;
}

void rewind (FILE *stream)
{
	stream->header.offset_lo = 0;
	stream->header.offset_hi = 0;

}

int seek (FILE *stream, long num_bytes, int origin )
{

	switch(origin) {

	case SEEK_SET:
		break;
	case SEEK_CUR:
		break;
	case SEEK_END:
		break;

	}

	return 0;
}

size_t read (void *buffer,size_t num_bytes, size_t count, FILE *stream)
{

	return (count);
}

size_t write (const void *buffer,size_t num_bytes, size_t count, FILE *stream)
{

	return (count);
}


int error (FILE *stream )
{
	return 0;
}

int remove (const char *filename)
{


	return 0;
}





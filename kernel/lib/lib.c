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
 * w+		Cria um arquivo-texto para leitura/escrita
 * a+		Anexa ou cria um arquivo-texto para leitura/escrita
 * r+b		Abre um arquivo binário para leitura/escrita
 * w+b		Cria um arquivo binário para escrita/escrita
 * a+b		Anexa ou cria um arquivo binário para escrita/escrita
 *
 *	
 */

void free_file_table(FILE *fp)
{
	int i;
	if(!fp->table)return;

	for(i = 0; i <1024;i++) {

		if(!fp->table[i]) break;

		free((void*)fp->table[i]);
	}

	free(fp->table);
}

FILE *open (const char *filename,const char *mode) 
{

	if(mode[0] == '\0') return NULL;

	unsigned char flag = 0;

	unsigned int phys;


	FILE *stream;
	if ((mode[0] == 's') && (mode[1] == 't') && (mode[2] == 'd')) {

		stream = (FILE*)malloc(sizeof(FILE));
		setmem(stream,sizeof(FILE),0);

		flag = 2;

		//stream->flags = 1;
		stream->mode[0] = mode[0];
		stream->mode[1] = mode[1];
		stream->mode[2] = mode[2];
		if(mode[3] == 'i') {
			stream->flags = 2;
		}else if(mode[3] == 'o'){
	
			stream->flags = 3;


		}else if(mode[3] == 'r'){
	
			stream->flags = 4;


		}else 	stream->flags = 5;

		// 64 KiB

		int k = BUFSIZ/4096;
		if(BUFSIZ%4096) k++;
		alloc_pages(0,k,(VIRTUAL_ADDRESS *)&phys);

		stream->buffer = (unsigned char *) phys;
		setmem(stream->buffer,BUFSIZ,0);


		stream->bsize = BUFSIZ;


		return (FILE*) stream;
		
	}
	
	

	if ((mode[0] == 'r') && (mode[1] == 'x')) {

		stream = __vfs__;
		setmem(stream,sizeof(FILE),0);

		stream->mode[0] = 'r';
		stream->mode[1] = 'x';

		flag = 1;
		
	} else { 

		stream = (FILE*)malloc(sizeof(FILE));
		setmem(stream,sizeof(FILE),0);

	}



	stream->table = (unsigned int *)malloc(0x1000);
	setmem(stream->table,0x1000,0);


	FAT_DATA *data 	= (FAT_DATA*)malloc(sizeof(FAT_DATA));
	MBR *mbr	= (MBR*)malloc(0x200);
	VOLUME *volume	= (VOLUME*)malloc(sizeof(VOLUME));


	// FIXME Nelson tem que pensar em ler o mbr fora desta rotina
	if(read_sector(DEV,1,0,mbr) ) goto error;

	volume->lba_start = mbr->part[0].lba_start;
	volume->dev_num = DEV;



	//FAT TYPE
	FAT_BPB  *bpb  	= FatReadBPB(volume);

	if(bpb == NULL) goto error;

	// salve bpb
	stream->superblock = (unsigned char*) bpb;

	FAT_DIRECTORY *root =FatOpenRoot(bpb,data);

	if(root == NULL) { free(bpb);  goto error; }

	if( FatOpenFile(bpb,data,root,filename,ATTR_ARCHIVE,stream) ) {

		if((*mode == 'w') || (*mode == 'a') ) {

			if( FatCreateFile(bpb,data,root,filename,ATTR_ARCHIVE) ) { 
				free(bpb); free_pages(root); goto error; 
			}else {
				FAT_DIRECTORY *_root = FatUpdateRoot(bpb,data,root);
				if(_root == NULL) { 
					free(bpb); free_pages(root); goto error; 
				}

			}
			if( FatOpenFile(bpb,data,root,filename,ATTR_ARCHIVE,stream) ) { 
				free(bpb); free_pages(root); goto error; 
			}

		} else { free(bpb); free_pages(root); goto error; }
	}


	free(volume);
	free(mbr);
	free_pages(root);
	free(data);

	if(flag == 1)stream->buffer = (unsigned char*)__vfsbuf__;
	else if(flag == 2);
	else stream->buffer = (unsigned char*)malloc(BUFSIZ);
	return (FILE*) stream;

error:
	free_file_table(stream);
	free(volume);
	free(mbr);
	free(data);
	if(!flag)free(stream);
	return NULL;
}

int close (FILE *stream) 
{

	if(stream->mode[0] == '\0') return 0;

	if((stream->mode[0] == 'r') && (stream->mode[1] == 'x')) {


		stream->flags = 0;
		return EOF;


	}


	if ((stream->mode[0] == 's') \
	&& (stream->mode[1] == 't') && (stream->mode[2] == 'd')) {

		stream->flags = 0;
		free_pages(stream->buffer);
		free(stream);

		return EOF;
	}

	stream->flags = 0;

	free(stream->buffer);
	free(stream->superblock);
	free_file_table(stream);
	free(stream);

	return EOF; // successfull
}

int flush(FILE *stream) {

	// error
	/*if(stream->header.mode[0] == '\0') return EOF;


	// rx
	if((stream->header.mode[0] == 'r') && (stream->header.mode[1] == 'x')) {

		return 0;


	}

	// std
	if ((stream->header.mode[0] == 's') \
	&& (stream->header.mode[1] == 't') && (stream->header.mode[2] == 'd')) {


		return 0;
	}




	// default

	FAT_DATA *data 	= (FAT_DATA*)malloc(sizeof(FAT_DATA));


	if(stream->header.bpb == NULL) goto error;

	FAT_DIRECTORY *root =FatOpenRoot(stream->header.bpb,data);

	if(root == NULL) { goto error; }

	

	if(FatUpdateFile(stream->header.bpb,data,root,stream)) {
		free_pages(root); goto error;
	}


	free_pages(root);
	free(data);

	return 0;

error:
	free(data);*/

	
	return EOF;

}

int putc (int ch, FILE *stream) 
{
	unsigned char* buffer = (unsigned char*) (stream->buffer);
	unsigned int offset = stream->off;
	
	

	// error
	if(stream->mode[0] == '\0') return EOF;

	

	// std
	if ((stream->mode[0] == 's') \
	&& (stream->mode[1] == 't') && (stream->mode[2] == 'd')) {


		if(offset >= BUFSIZ) {

			setmem((unsigned char*)(buffer),BUFSIZ,0);
			stream->off = 0;
			stream->off2 = 0;
			return EOF;

		}

		// write character
		*(unsigned char*)(buffer + offset) = ch;

		// Update offset
		offset = stream->off +=1;

		return  (ch&0xff); // successfull
	}

	// Outros modos default lib C
	return (EOF);
}

int getc (FILE *stream)
{
	unsigned char* buffer = (unsigned char*) (stream->buffer);
	unsigned int offset2 = stream->off2;
	int ch;
	
	
	// error
	if(stream->mode[0] == '\0') return EOF;

	

	// std
	if ((stream->mode[0] == 's') \
	&& (stream->mode[1] == 't') && (stream->mode[2] == 'd')) {


		if(offset2 >= BUFSIZ) {

			stream->off2 = 0;
			return EOF;

		}

		ch = *(unsigned char*)(buffer + offset2);

		// Update offset
		if(ch) offset2 = stream->off2 +=1;

		return ch; // successfull
	}

	// Outros modos default lib C

	return EOF;

}

size_t write (const void *buffer,size_t num_bytes, size_t count, FILE *stream)
{
	
	return (0);
}

size_t read (void *buffer,size_t num_bytes, size_t count, FILE *stream)
{	unsigned char *cache = (unsigned char*) (stream->buffer);
	unsigned char *__buffer__ = (unsigned char*) (buffer);
	unsigned __count__ = stream->count;
	unsigned int offset = stream->off;
	unsigned int total_blocks = stream->tnb;
	unsigned int bps = stream->bps;
	unsigned int local_offset;
	unsigned tmp_count = (count*num_bytes);
	unsigned tmp2_count = 0;
	int i;

	unsigned int *block;
	unsigned sector_start;
	int block_idex;
	int table_idex;
	int r;

	int operation = ((num_bytes*count)/(bps*__count__));
	if(((num_bytes*count)%(bps*__count__)))operation++;
	
	if(!total_blocks) return EOF;

	
	for(i = 0;i < operation;i++) {
		if(offset >= (total_blocks*bps*__count__)) {
			// FINAL DO ARQUIVO, leu todos os blocos
			return 0;

		}

		// FIXME
		r = stream->off / stream->bsize;
		table_idex = r / 1024;
		block_idex = r % 1024;

		block= (unsigned int *) stream->table[table_idex];
		sector_start = block[block_idex];

		local_offset = offset%(bps*__count__);

		// read
		if( read_sector(stream->devn, __count__, sector_start,cache) ) 
			return (0);

		// write character
		if(tmp_count >= (bps*__count__)) { 
			tmp2_count = bps*__count__; 
			tmp_count -= bps*__count__;

		} else { 
			tmp2_count = tmp_count;

		}

		copymem(__buffer__ + (i*bps*__count__) ,cache + local_offset,tmp2_count);

		if(stream->off < 0xFFFFFFFF) { 
			// update offset
			offset = stream->off += (tmp2_count);
			
		}
		else {return 0;/* Arquivo superior a 4GiB*/};

	}

	return ((i-1)*(bps*__count__) + tmp2_count);


	return (count);
}

int eof (FILE *stream)
{

	return 0;
}

void rewind (FILE *stream)
{
	stream->off2 = stream->off = 0;

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


int error (FILE *stream )
{
	return 0;
}

int remove (const char *filename)
{


	return 0;
}





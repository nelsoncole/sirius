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

FILE *open (const char *filename,const char *mode) 
{

	if(mode[0] == '\0') return NULL;

	unsigned char flag = 0;


	FILE *stream;
	if ((mode[0] == 's') && (mode[1] == 't') && (mode[2] == 'd')) {

		stream = (FILE*)malloc(0x1000);

		setmem(stream,0x1000,0);

		flag = 2;

		stream->header.flag = 1;
		stream->header.mode[0] = mode[0];
		stream->header.mode[1] = mode[1];
		stream->header.mode[2] = mode[2];
		if(mode[3] == 'i') {
			stream->header.attr = 2;
		}else if(mode[3] == 'o'){
	
			stream->header.attr = 3;


		}else 	stream->header.attr = 4;

		// 64 KiB
		alloc_pages(0,16,(VIRTUAL_ADDRESS *)&stream->header.buffer);

		setmem((void*)(stream->header.buffer),0x10000,' ');


		return (FILE*) stream;
		
	}
	
	

	if ((mode[0] == 'r') && (mode[1] == 'x')) {

		stream = __vfs__;

		stream->header.mode[0] = 'r';
		stream->header.mode[1] = 'x';

		flag = 1;
		
	} else stream = (FILE*)malloc(0x1000);



	FAT_DATA *data 	= (FAT_DATA*)malloc(sizeof(FAT_DATA));
	MBR *mbr	= (MBR*)malloc(0x200);
	VOLUME *volume	= (VOLUME*)malloc(sizeof(VOLUME));
	

	setmem(stream,0x1000,0);

	// FIXME Nelson tem que pensar em ler o mbr fora desta rotina
	if(read_sector(DEV,1,0,mbr) ) goto error;

	volume->lba_start = mbr->part[0].lba_start;
	volume->dev_num = DEV;



	//FAT TYPE
	FAT_BPB  *bpb  	= FatReadBPB(volume);

	if(bpb == NULL) goto error;

	// salve bpb
	stream->header.bpb = bpb;

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


	stream->header.flag = 1;
	stream->header.current 	= NULL;
	stream->header.next 	= NULL;
	if(flag == 1)stream->header.buffer = (unsigned int)__vfsbuf__;
	else if(flag == 2);
	else stream->header.buffer = (unsigned int)malloc(0x10000);
	return (FILE*) stream;

error:
	free(volume);
	free(mbr);
	free(data);
	if(!flag)free(stream);
	return NULL;
}

int close (FILE *stream) 
{

	if(stream->header.mode[0] == '\0') return 0;

	if((stream->header.mode[0] == 'r') && (stream->header.mode[1] == 'x')) {


		stream->header.flag = 0;
		return EOF;


	}


	if ((stream->header.mode[0] == 's') \
	&& (stream->header.mode[1] == 't') && (stream->header.mode[2] == 'd')) {

		stream->header.flag = 0;
		free_pages((void*)stream->header.buffer);
		free(stream);


		return EOF;
	}




	FAT_DATA *data 	= (FAT_DATA*)malloc(sizeof(FAT_DATA));


	if(stream->header.bpb == NULL) goto error;

	FAT_DIRECTORY *root =FatOpenRoot(stream->header.bpb,data);

	if(root == NULL) { goto error; }

	

	if(FatUpdateFile(stream->header.bpb,data,root,stream)) {
		free(root); goto error;
	}

	free(root);
error:
	free(data);


	stream->header.flag = 0;
	free((void*)(stream->header.buffer));
	free(stream->header.bpb);
	free(stream);

	return EOF; // successfull
}

int flush(FILE *stream) {

	// error
	if(stream->header.mode[0] == '\0') return EOF;


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
	free(data);

	
	return EOF;

}

int putc (int ch, FILE *stream) 
{
	unsigned char* buffer = (unsigned char*) (stream->header.buffer);
	unsigned dev_n = stream->header.dev;
	unsigned count = stream->header.count;
	unsigned int offset = stream->header.offset;
	unsigned int total_blocks = stream->header.blocks;
	unsigned int *block	= (unsigned int*)(stream->block);
	unsigned int bps = stream->header.bps;
	unsigned int lba_start;
	unsigned int local_offset;
	
	

	// error
	if(stream->header.mode[0] == '\0') return EOF;

	

	// std
	if ((stream->header.mode[0] == 's') \
	&& (stream->header.mode[1] == 't') && (stream->header.mode[2] == 'd')) {


		if(offset >= 65536) {

			setmem((unsigned char*)(buffer),65536,0);
			stream->header.offset = 0;
			stream->header.offset2 = 0;
			return EOF;

		}

		// write character
		*(unsigned char*)(buffer + offset) = ch;

		// Update offset
		offset = stream->header.offset +=1;

		return  (ch&0xff); // successfull
	}



	if(!total_blocks) return EOF;


	// Outros modos default lib C


	// FAT

	if(offset >= (total_blocks*bps*count)) {
		// new block 
		AddFAT(stream,stream->header.bpb,stream->block[total_blocks-1]);
		// Update
		total_blocks = stream->header.blocks;

	}

	lba_start = block[offset/bps/count] + (offset/bps%count);
	
	local_offset = offset%bps;

	if(read_sector(dev_n,1,lba_start,buffer)) return EOF;

	// write character
	*(unsigned char*)(buffer + local_offset) = ch;

	// gravar no disco
	if(write_sector(dev_n,1,lba_start,buffer)) return EOF;
	
	if(offset >= stream->header.size )stream->header.size += 1;

	if(stream->header.offset < 0xFFFFFFFF) {
		// Update offset
		offset = stream->header.offset +=1;
	}
	else {return EOF;/* Arquivo superior a 4GiB*/};

	return  (ch&0xff); // successfull

	// unused
	return (count);
}

int getc (FILE *stream)
{
	unsigned char* buffer = (unsigned char*) (stream->header.buffer);
	unsigned dev_n = stream->header.dev;
	unsigned count = stream->header.count;
	unsigned int offset = stream->header.offset;
	unsigned int offset2 = stream->header.offset2;
	unsigned int total_blocks = stream->header.blocks;
	unsigned int *block	= (unsigned int*)(stream->block);
	unsigned int bps = stream->header.bps;
	unsigned int lba_start;
	unsigned int local_offset;
	int ch;
	
	
	// error
	if(stream->header.mode[0] == '\0') return EOF;

	

	// std
	if ((stream->header.mode[0] == 's') \
	&& (stream->header.mode[1] == 't') && (stream->header.mode[2] == 'd')) {


		if(offset2 >= 65536) {

			stream->header.offset2 = 0;
			return EOF;

		}

		ch = *(unsigned char*)(buffer + offset2);

		// Update offset
		if(ch) offset2 = stream->header.offset2 +=1;

		return ch; // successfull
	}




	if(!total_blocks) return EOF;

	// Outros modos default lib C


	// FAT

	if(offset >= (total_blocks*bps*count)) {
		// FINAL DO ARQUIVO, leu todos os blocos
		return EOF;

	}

	// FINAL DO ARQUIVO, leu todos os bytes
	if(offset >= stream->header.size) return EOF;

	lba_start = block[offset/bps/count] + (offset/bps%count);
	
	local_offset = offset%bps;

	if(read_sector(dev_n,1,lba_start,buffer)) return EOF;

	if(stream->header.offset < 0xFFFFFFFF){
		// Update offset
		offset = stream->header.offset +=1;
	}
	else {return EOF;/* Arquivo superior a 4GiB*/};

	return *(unsigned char*)(buffer + local_offset); // successfull

	// unused
	return (count);

}

size_t write (const void *buffer,size_t num_bytes, size_t count, FILE *stream)
{
	unsigned char *cache = (unsigned char*) (stream->header.buffer);
	unsigned char *__buffer__ = (unsigned char*) (buffer);
	unsigned dev_n = stream->header.dev;
	unsigned __count__ = stream->header.count;
	unsigned int offset = stream->header.offset;
	unsigned int total_blocks = stream->header.blocks;
	unsigned int *block	= (unsigned int*)(stream->block);
	unsigned int bps = stream->header.bps;
	unsigned int lba_start;
	unsigned int local_offset;
	unsigned tmp_count = (count*num_bytes);
	unsigned tmp2_count = 0;
	int i;
	int operation = ((num_bytes*count)/bps);
	if(((num_bytes*count)%bps))operation++;

	if(!total_blocks) return EOF;

	for(i = 0;i < operation;i++) {
		if(offset >= (total_blocks*bps*__count__)) {
			// new block 
			AddFAT(stream,stream->header.bpb,stream->block[total_blocks-1]);
			// Update
			total_blocks = stream->header.blocks;

		}

		lba_start = block[offset/bps/__count__] + (offset/bps%__count__);
		local_offset = offset%bps;


		if(read_sector(dev_n,2,lba_start,cache)) return 0;

		// write character
		if(tmp_count >= bps) { 
			tmp2_count = bps; 
			tmp_count  -= bps;

		}
		else { 
			tmp2_count = tmp_count;

		}

		copymem(cache + local_offset,__buffer__ + (i*bps),tmp2_count);

		// gravar no disco
		if(write_sector(dev_n,2,lba_start,cache)) return 0;
		
		if((offset >= stream->header.size) || ((offset+tmp2_count) >= stream->header.size ) )
			stream->header.size += (tmp2_count);

		if(stream->header.offset < 0xFFFFFFFF) { 
			// update offset
			offset = stream->header.offset += (tmp2_count);
			
		}
		else {return 0;/* Arquivo superior a 4GiB*/};

	}

	return ((i-1)*bps + tmp2_count);
}

size_t read (void *buffer,size_t num_bytes, size_t count, FILE *stream)
{	unsigned char *cache = (unsigned char*) (stream->header.buffer);
	unsigned char *__buffer__ = (unsigned char*) (buffer);
	unsigned dev_n = stream->header.dev;
	unsigned __count__ = stream->header.count;
	unsigned int offset = stream->header.offset;
	unsigned int total_blocks = stream->header.blocks;
	unsigned int *block	= (unsigned int*)(stream->block);
	unsigned int bps = stream->header.bps;
	unsigned int lba_start;
	unsigned int local_offset;
	unsigned tmp_count = (count*num_bytes);
	unsigned tmp2_count = 0;
	int i;
	int operation = ((num_bytes*count)/bps);
	if(((num_bytes*count)%bps))operation++;
	
	if(!total_blocks) return EOF;

	
	for(i = 0;i < operation;i++) {
		if(offset >= (total_blocks*bps*__count__)) {
			// FINAL DO ARQUIVO, leu todos os blocos
			return 0;

		}

		lba_start = block[(offset/bps)/__count__] + ((offset/bps)%__count__);
		local_offset = offset%bps;


		if(read_sector(dev_n,2,lba_start,cache)) return 0;

		// write character
		if(tmp_count >= bps) { 
			tmp2_count = bps; 
			tmp_count  -= bps;

		}
		else { 
			tmp2_count = tmp_count;

		}

		copymem(__buffer__ + (i*bps) ,cache + local_offset,tmp2_count);

		if(stream->header.offset < 0xFFFFFFFF) { 
			// update offset
			offset = stream->header.offset += (tmp2_count);
			
		}
		else {return 0;/* Arquivo superior a 4GiB*/};

		
		
	}

	return ((i-1)*bps + tmp2_count);


	return (count);
}

int eof (FILE *stream)
{

	return 0;
}

void rewind (FILE *stream)
{
	stream->header.offset = 0;

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





#include <stdio.h>
#include <stdlib.h>

int main() {

	//printf("FIXME: bug no gserver em gui_exit(), falha quando encerramos multiplas janelas, precisa de um spinlock \n");




	FILE *fd = fopen("/dev/sdax","r+b"); 

	int var = 0;

	if(!fd) {

		printf("Open error\n");

	} else {

		fseek (fd,510, SEEK_SET);
		var = (fgetc(fd)&0xff);
		var |= (fgetc(fd)&0xff) << 8;

		printf("Read word 511 \"/dev/sdax\" bootsig 0x%x",var);

		fclose(fd);

	}

	

	return 0;
}








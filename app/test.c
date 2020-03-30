#include <stdio.h>
#include <stdlib.h>

int main() {

	//printf("FIXME: bug no gserver em gui_exit(), falha quando encerramos multiplas janelas, precisa de um spinlock \n");




	FILE *fd = fopen("/dev/sda","r+b");


	


	printf("Por 0x10000 %d Por 0 %d Por Numero qualquer %d",0x10000%0x10001,0%0x10000,0x10007%0x10000);


	return 0;
 

	int var = 0;

	if(!fd) {

		printf("Open error\n");

	} else {



		fseek (fd,508, SEEK_SET);
		if(fread(&var,1,4,fd) == 4 )
		{
			printf("Read Dword 508 em \"/dev/sda\"\nBootsig = 0x%x",var);

		} else printf("fread() error");

		fclose(fd);

	}

	

	return 0;
}








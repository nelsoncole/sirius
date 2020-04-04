#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char **argv) {



	printf("Hello, world!\n");

	return (0);


	/*char _str[] = "O soldado de verao, o patriota da luz do dia";
	char *p;
	p = strtok(_str," ");

	printf("%s",p);


	do {

		p = strtok('\0', ", ");

		if(p) printf("|%s",p);


	}while(p);*/


	printf("%s\n",strchr("isto e um teoste",'o'));
	printf("%s\n",strrchr("isto e um teoste",'o'));

	return 0;

	char str[] = "This is Sirius Operating System.";

	size_t len = sizeof(str) - 1;


	FILE *fp = fopen("file.txt","w");

	if(!fp) {

		printf("fopen Error\n");

	} else {

		if(fwrite(str, 1, len, fp) == len )
		{
			fputs("successfull",stdout);
			
		} else printf("fwrite() error");


		fclose(fp);

	}

	return 0;
}

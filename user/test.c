#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {


	printf("argc %d %s",argc,argv[0]);
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

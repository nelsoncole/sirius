#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <io.h>

int main(int argc, char **argv) {



	char *p1 = malloc(4);
	char *p2 = malloc(4);
	char *p3 = malloc(4);
	char *p4 = malloc(4);

	free(p1);
	free(p2);
	free(p3);
	free(p4);

	char *p5 = malloc(4);


	printf("p1 0%x\n",p1);
	printf("p2 0%x\n",p2);
	printf("p3 0%x\n",p3);
	printf("p4 0%x\n",p4);
	printf("p5 0%x\n",p5);

	free(p5);

	return 0;


	char *wallpaper = (char *) malloc(0x400000); // 4 MiB
	int size;

	unsigned int width, height, sig;

	char *_wallpaper = wallpaper;
	char *p;

	FILE *f = fopen("b.ppm","r");
	if(f == NULL) printf("error: fopen\n");
	else {

	
		fseek(f, 0,SEEK_END);
		size = (int) ftell(f);
		fseek(f, 0,SEEK_SET);

		if(fread(wallpaper, 1, size, f) != size) printf("error: fread\n");
		else {

			p = strtok(_wallpaper,"\n ");

			printf("%s\n",p);

			p = strtok('\0', "\n ");
			width = strtoul (p,NULL, 10);
			p = strtok('\0', "\n ");
			height = strtoul (p,NULL, 10);
			p = strtok('\0', "\n ");
			sig = strtoul (p,NULL, 10);
			_wallpaper = strtok('\0', "\n"); 

			printf("width %d height %d sig %d\n",width, height,sig);

		
			char a[4];
			char rgb[4];

			int off = 0;
			for(int y=0;y < height;y++) {

				for(int x=0;x < width;x++) {

					*(unsigned int*)a =  (*(unsigned int*)((_wallpaper) + (3*off++))) &0xffffff;

					rgb[0] = a[0];
					rgb[1] = a[1];
					rgb[2] = a[2];
					rgb[3] = 0xff;
					PutPixelBuff(x+200,y+100,*(unsigned int*)rgb,G->BankBuffer);

				}
			}


			app_refreshrate();
			
		}

	}


	free(wallpaper);

	return 0;
}

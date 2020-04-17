#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) 
{

	FILE *fp1, *fp2;


	if(argc < 2) return (0);


	char *stage0 = (char *)malloc(512); //  512


	// Open fp
	if((fp1 = fopen("stage0.bin","r+b")) == NULL) {
		printf("error: stage0.bin, file not found\n");
		goto end; 
	}


	if( ( fread(stage0,1,512,fp1) ) != 512) {
		printf("error: read  stage0\n");
		fclose(fp1);
		goto end;	
	}


	// Open fp1
	if((fp2 = fopen(argv[1],"r+b")) == NULL) {
		printf("error: %s file not found\n",argv[1]);
		fclose(fp1);
		goto end; 
	}

	//Escrevendo o sector de boot
	if(( fwrite(stage0,1,0x1B8,fp2) ) != 0x1B8) {
		printf("error: write fail");
		fclose(fp1);
		fclose(fp2);
		goto end;	
	}


	

	fclose(fp1);
	fclose(fp2);

end:

	free(stage0);
	
	
	return 0;

}

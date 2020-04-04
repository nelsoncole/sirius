#include <string.h>
#include <stdio.h>

char *us1;

char *strtok(char * restrict s1,const char * restrict s2)
{


	printf("strtok( defino no stdlib.h )\n");
	for(;;);


	static int first = 0;

	if(!first) { 

		us1 = (char *) s1;
		first = 1;

		if(*s1 == '\0') return (us1);


	} else {

		if(!*us1) return NULL;
		
	}

	unsigned char *us2 = (unsigned char *) s2;

	char *d = (char *) us1;
	char *rc = d;

	size_t len = strlen((char *)us2);
	int i;
	int fl = 1;

	for( ;*d != '\0' ;d++) 
	{
		if ( *d == *us2 )
		{
			for(i=0;i<len;i++) 
			{
				if ( (*d == *us2++) && (fl) ){ 
					*d++ = '\0';
				}
				else fl = 0;

				 
			}

			break;


		} 
		

	}

	us1 = (char *)d;


	printf("strtok()");
	for(;;);


	return (rc);

}

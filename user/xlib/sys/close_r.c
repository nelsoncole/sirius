#include <sys/sys.h>
#include <stdlib.h>

extern void free_file_table(FILE *fp);

int close_r(FILE *fp) {

	if(!fp) return (0);

	if((fp->flags & 0x20) && ((fp->mode[0] == 'w') ||\
	(fp->mode[0] == 'a') || (fp->mode[1] == '+'))) flush_r(fp);


	fp->flags = 0;
	free(fp->buffer);
	free(fp->superblock);
	free_file_table(fp);
	free(fp);

	// EOF
	return (-1);
}

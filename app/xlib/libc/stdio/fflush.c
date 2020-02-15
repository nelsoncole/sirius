#include <sys/sys.h>
#include <stdio.h>


int fflush(FILE *fp) {

	return flush(fp);

}

#include <stdio.h>     /* printf, sprintf */
#include "splitter.h"  /* SplitFile, JoinFiles */

int SplitFile(char const * filename, char const * output, size_t size) {
    char current_file_name[256];
    int file_count=0,i=0;
	char * in_buffer;
	
	in_buffer = (char*) malloc (size + 1);

{	
    /* print in to a string. The format is string(%s) 
     * and a 4-digit integer padded with 0's on the left, i.e.
     * 1 is printed as 0001, 123 as 0123, note that 12345 is printed as 12345 
     * sprintf(current_file_name,"%s%04lu\n",output,++file_count); 
     */
}

	free (in_buffer);
	
return 0;
}

int JoinFiles(char** filenames, int num_files, char const * output) {
	int i;
    printf("join\n");
    for ( i=0; i<num_files; ++i ) {
        printf("%s\n",*filenames[i] );
    }
    printf("into %s\n",output);
    return 0;
}

/*
int main () 
{
	printf("Calling Split");
	SplitFile("File","Fileout",10);

	return 0;
}
*/


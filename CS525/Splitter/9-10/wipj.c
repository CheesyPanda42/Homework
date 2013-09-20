
#include <stdio.h>
#include <stdlib.h>

/*int SplitFile(char const *  filename,  char const * output, size_t size);*/
int JoinFiles(char** filenames, int num_files, char const * output);

int main(int argc, char **argv)
{
	char ** filenames;
	int i;
	filenames = (char**) malloc (100);
	
	JoinFiles(filenames, 5, "combined_");

	
	return 0;
}

int JoinFiles(char** filenames, int num_files, char const * output)
{
	int i;
	int j = 0;
	char* in_buffer;
    printf("\njoin\n");
	FILE *combined_file;
	FILE *input_file;
	int bytes_read;
	
	
	in_buffer = malloc(100);
	memset(in_buffer,NULL,99);
	
	if(filenames)
	{
		for ( i=0; i<num_files; ++i ) 
		{
			/*fputs("IN for loop\n", stderr);*/
			filenames[i] = malloc(32);
			/*fputs("allocated memory\n", stderr);*/
			sprintf(filenames[i],"%s%04lu", "test_", ++j);
			/*fputs("named files\n", stderr);*/
			printf("%d:%s\n",i,filenames[i] );
		}
	}
	
	combined_file = fopen(output, "wb");
	printf("file opened\n");
	
	for (i = 0; i < num_files; i++)
	{
		memset(in_buffer, NULL, 99);
		input_file = fopen(filenames[i],"rb");
		bytes_read = fread (in_buffer,1,100,input_file);
		fwrite (in_buffer, 1, bytes_read,combined_file);
		printf(in_buffer);
	}
	
	
	fclose(combined_file);
    printf("into %s\n",output);
    return 0;
}

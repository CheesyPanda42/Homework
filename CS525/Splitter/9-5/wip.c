
#include <stdio.h>
#include <stdlib.h>

int SplitFile(char const *  filename,  char const * output, size_t size);
int JoinFiles(char** filenames, int num_files, char const * output);

int main(int argc, char **argv)
{
	char filename[256];
	
	printf("Filename?\n");
	scanf("%s",filename);
	
	printf("Filename: %s\n\n",filename);
	
	SplitFile (filename, "test", 100);
	
	return 0;
}


int SplitFile(char const * filename, char const * output, size_t size)
{
	char current_file_name[256];
    int file_count=0,i=0;
	char * in_buffer;
	int bytes_read;
	int bytes_written;
	
	
	in_buffer = (char*) malloc(size+1);
	for (i = 0; i < size; i++)
	{
		in_buffer[i] = 0;
	}
	
	printf("In SplitFile\n\n");
	
	FILE *input = fopen(filename, "rb");
	FILE *outputfile = fopen(output, "wb");
	
	printf("File %s opened\n\n", filename);
	printf("File %s opened\n\n", outputfile);
	
	while (bytes_read != EOF){
		bytes_read = fread(in_buffer, 1, size, input);
		fwrite(in_buffer,1,size,outputfile);
	}
	printf("%d\n\n",bytes_read);
	
	/*sprintf(current_file_name,"%s%04lu\n",output,++file_count);*/
	
	
	fclose(input);
	fclose(outputfile);
	printf("File %s closed\n\n", filename);
	printf("File %s closed\n\n", outputfile);
	free (in_buffer);
	return 0;
}

int JoinFiles(char** filenames, int num_files, char const * output)
{
	int i;
    printf("join\n");
    for ( i=0; i<num_files; ++i ) {
        printf("%s\n",*filenames[i] );
    }
    printf("into %s\n",output);
    return 0;
}

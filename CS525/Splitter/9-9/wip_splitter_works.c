
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
	
	SplitFile (filename, "test_", 75);
	
	return 0;
}


int SplitFile(char const * filename, char const * output, size_t size)
{
	char current_file_name[256];
    int file_count=0,i=0;
	char * in_buffer;
	int bytes_read;
	int bytes_written;
	FILE *inputfile;
	FILE *outputfile;
	
	in_buffer = (char*) malloc(size+1);
	
	
	
	memset(in_buffer,NULL,size);
	memset(current_file_name,NULL,255);
	
	printf("In SplitFile\n\n");
	
	inputfile = fopen(filename, "rb");
	/*FILE *outputfile = fopen(output, "wb");*/
	
	printf("File %s opened\n\n", filename);
	/*printf("File %s opened\n\n", outputfile);*/
	
	do{
		sprintf(current_file_name,"%s%04lu", output, ++file_count);
		bytes_read = fread(in_buffer, 1, size, inputfile);
		if (bytes_read >0)
		{
			outputfile = fopen(current_file_name, "wb");
			fwrite(in_buffer,1,bytes_read,outputfile);
		}
		memset(in_buffer,NULL,size);
		printf("%d\n\n",bytes_read);
		fclose(outputfile);
	} while (bytes_read > 0);
	
	printf("%d\n\n",bytes_read);
	printf("%s", in_buffer);
	
	/*sprintf(current_file_name,"%s%04lu\n",output,++file_count);*/
	
	
	fclose(inputfile);
	
	printf("File %s closed\n\n", filename);
	printf("File %s closed\n\n", outputfile);
	
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

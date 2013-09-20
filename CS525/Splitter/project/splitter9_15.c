#include <stdio.h>     /* printf, sprintf */
#include "splitter.h"  /* SplitFile, JoinFiles */

#define BUFF_SIZE 4096
#define FILENAME_MAX_SIZE 256

/* NEED TO IMPLEMENT THE 4K MAX BUFFER SIZE*/

int SplitFile(char const * filename, char const * output, size_t size) {
	char current_file_name[FILENAME_MAX_SIZE];
    int file_count=0;/*,i=0;*/
	char * in_buffer;
	int bytes_read;
	/*int bytes_written;*/
	FILE *inputfile;
	FILE *outputfile;
	
	in_buffer = (char*) malloc(min(size+1,BUFF_SIZE));
	
	memset(in_buffer,0,size);
	memset(current_file_name,0,FILENAME_MAX_SIZE-1);
	
	printf("In SplitFile\n\n");
	
	inputfile = fopen(filename, "rb");
	/*FILE *outputfile = fopen(output, "wb");*/
	/*outputfile = fopen("dummy","wb");*/
	
	printf("File %s opened\n\n", filename);
	/*printf("File %s opened\n\n", outputfile);*/
	
	do{
		sprintf(current_file_name,"%s%04i", output, ++file_count);
		bytes_read = fread(in_buffer, 1, size, inputfile);
		if (bytes_read >0)
		{
			outputfile = fopen(current_file_name, "wb");
			fwrite(in_buffer,1,bytes_read,outputfile);
		}
		memset(in_buffer,0,size);
		printf("%d\n\n",bytes_read);
		fclose(outputfile);
	} while (bytes_read > 0);
	
	printf("%d\n\n",bytes_read);
	printf("%s", in_buffer);
	
	/*sprintf(current_file_name,"%s%04lu\n",output,++file_count);*/
	
	
	fclose(inputfile);
	
	printf("File %s closed\n\n", filename);
	printf("File %s closed\n\n", (char*)outputfile);
	free (in_buffer);
	return 0;
	
}

int JoinFiles(char** filenames, int num_files, char const * output) {
	int i;
	/*int j = 0;*/
	char* in_buffer;
	/*char current_file_name[FILENAME_MAX_SIZE];*/
    
	FILE *combined_file;
	FILE *input_file;
	int bytes_read;
	
	printf("\njoin\n");
	
	in_buffer = (char*)malloc(BUFF_SIZE);
	memset(in_buffer,0,BUFF_SIZE-1);
	
	if(filenames)
	{
		for ( i=0; i<num_files; ++i ) 
		{
			/*printf("%d:%s\n",i,filenames[i] );*/
			/*fputs("IN for loop\n", stderr);*/
			/*filenames[i] = malloc(FILENAME_MAX);*/
			/*fputs("allocated memory\n", stderr);*/
			/*sprintf(filenames[i],"%s%04lu", "test_", ++j);*/
			
			/*fputs("named files\n", stderr);*/
			printf("%d:%s\n",i,filenames[i] );
		}
	}
	
	combined_file = fopen(output, "wb");
	printf("file opened\n");
	
	for (i = 0; i < num_files; i++)
	{
		memset(in_buffer,0, BUFF_SIZE-1);
		input_file = fopen(filenames[i],"rb");
		bytes_read = fread (in_buffer,1,BUFF_SIZE,input_file);
		fwrite (in_buffer, 1, bytes_read,combined_file);
		fputs(in_buffer,stderr);
	}
	
	
	fclose(combined_file);
    printf("into %s\n",output);
	free (in_buffer);
    return 0;
}



/******************************************************************************/
/*!
\file   splitter.c
\author Cory Prelerson
\par    email: cory.prelerson@digipen.edu
\par    DigiPen login: cory.prelerson
\par    Course: CS525
\par    Assignment #1 - File Splitter
\date   9.15.2013
\brief  
  This is the implementation file for the File Splitter program. 
  
  Operations are:
	- Split a large file into several smaller files
	- Join several smaller files into one larger file
  
*/
/******************************************************************************/

#include <stdio.h>     /* printf, sprintf */
#include "splitter.h"  /* SplitFile, JoinFiles */
/********************************************************************/
/*!
		- BUFF_SIZE - maximum size of input buffer 
		
*/
/*******************************************************************/
#define BUFF_SIZE 4096
/*!
		- FILENAME_MAX_SIZE- maximum size for input or output file name
*/
#define FILENAME_MAX_SIZE 256


/********************************************************************/
/*!
	Splits larger file into several small chunks. Reads into a buffer
	of size min(size, \p BUFF_SIZE), then writes buffer out.
	
	\param filename
		File name for file to be split
	\param output
		File name prefix for smaller files
	\param size
		Size of chunks for file to be split into.
*/
/*******************************************************************/
int SplitFile(char const * filename, char const * output, size_t size) {
	char current_file_name[FILENAME_MAX_SIZE];
    int file_count=0;
	char * in_buffer;
	int bytes_read;
	FILE *input_file;
	FILE *outputfile;
	
	if (size > BUFF_SIZE) size = BUFF_SIZE; /*! Input buffer size = min(size, BUFF_SIZE)*/
	
	in_buffer = (char*) malloc(size +1);
	if(!in_buffer)
	{
		return(ErrorHan(E_NO_MEMORY));
	}
	
	memset(in_buffer,0,size);
	memset(current_file_name,0,FILENAME_MAX_SIZE-1);
	
	printf("In SplitFile\n\n");
	
	input_file = fopen(filename, "rb");
	
	if(!input_file)
	{	
		printf("Input file does not exist.\n");
		return(ErrorHan(E_BAD_SOURCE));
	}
	printf("File %s opened\n\n", filename);
	
	/* Main loop.
	 * Reads in 'size' bits to in_buffer. If at least 1 element was read, in_buffer is written
	 * out to current output file. Stops when nothing is read in from input file
	 */
	do{
		sprintf(current_file_name,"%s%04i", output, ++file_count);
		bytes_read = fread(in_buffer, 1, size, input_file);
		if (bytes_read >0)
		{
			outputfile = fopen(current_file_name, "wb");
			if(!outputfile)
			{
				return (ErrorHan(E_BAD_DESTINATION));
			}
			fwrite(in_buffer,1,bytes_read,outputfile);
			printf ("Writing to %s...\n",current_file_name);
			fclose(outputfile);
		}
		memset(in_buffer,0,size);
	} while (bytes_read > 0);
	printf("Done. %d files created\n", --file_count);
	
	fclose(input_file);
	free (in_buffer);
	return 0;
	
}


/********************************************************************/
/*!
	Joins several smaller files into one larger one. Reads from input 
	file into buffer, then writes buffer out to output file.
	
	\param filenames
		List of file names to be joined.
	\param num_files
		The number of files that are to be joined.
	\param output
		Name of the file created by joining the smaller files.
*/
/*******************************************************************/
int JoinFiles(char** filenames, int num_files, char const * output) {
	int i;
	/*int j = 0;*/
	char* in_buffer;
	/*char current_file_name[FILENAME_MAX_SIZE];*/
    
	FILE *combined_file;
	FILE *input_file;
	int bytes_read;
	
	printf("\nJoin Files\n");
	
	in_buffer = (char*)malloc(BUFF_SIZE);
	if(!in_buffer)
	{
		return(ErrorHan(E_NO_MEMORY));
	}
	memset(in_buffer,0,BUFF_SIZE-1);
	
	if(filenames)
	{
		for ( i=0; i<num_files; ++i ) 
		{
			printf("%d:%s\n",i,filenames[i] );
		}
	}
	
	combined_file = fopen(output, "wb");
	if (!combined_file)
	{
		return (ErrorHan(E_BAD_DESTINATION));
	}
	printf("output file opened\n");
	
	for (i = 0; i < num_files; i++)
	{
		memset(in_buffer,0, BUFF_SIZE-1);
		input_file = fopen(filenames[i],"rb");
		if(!input_file)
		{	
			return(ErrorHan(E_BAD_SOURCE));
		}
		bytes_read = fread (in_buffer,1,BUFF_SIZE,input_file);
		fwrite (in_buffer, 1, bytes_read,combined_file);
		fclose(input_file);
	}
	
	fclose(combined_file);
    printf("into %s\n",output);
	free (in_buffer);
    return 0;
}

/********************************************************************/
/*!
	Handles printing error messages and returning error codes. Used 
	as a return value back to the driver.
	
	\param error_code
		Integer corresponding to error encountered.
	\return error_code
		Passes error code back as return value.
*/
/*******************************************************************/

int ErrorHan (int error_code)
{
	switch (error_code)
	{
			case E_BAD_SOURCE:
					printf("ERROR: Input file could not be opened.\n");
					break;
			case E_BAD_DESTINATION:
					printf("ERROR: Output file could not be opened.\n");
					break;
			case E_NO_MEMORY:
					printf("ERROR: Cannot allocate memory.\n");
					break;
	}
	return (error_code);
}
/******************************************************************************/
/*!
\file   splitter.h
\author Cory Prelerson
\par    email: cory.prelerson@digipen.edu
\par    DigiPen login: cory.prelerson
\par    Course: CS525
\par    Assignment #1 - File Splitter
\date   9.15.2013
\brief  
  This is the header file for the File Splitter program. 
  
  Operations are:
	- Split a large file into several smaller files
	- Join several smaller files into one larger file
  
*/
/******************************************************************************/
#ifndef SPLITTER_H
#define SPLITTER_H
#include <stddef.h> /* size_t definition */
#include <stdlib.h>
#include <string.h>

enum 
{E_BAD_SOURCE=1, E_BAD_DESTINATION, E_NO_MEMORY, E_NO_ACTION, E_SMALL_SIZE};



#ifdef __cplusplus
extern "C" {
#endif
int SplitFile(char const *  filename,  char const * output, size_t size);
int JoinFiles(char** filenames, int num_files, char const * output);
int ErrorHan (int error_code);
#ifdef __cplusplus
}
#endif


#endif 


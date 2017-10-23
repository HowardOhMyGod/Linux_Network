/*
 * lookup1 : straight linear search through a local file
 * 	         of fixed length records. The file name is passed
 *	         as resource.
 */
#include <string.h>
#include <ctype.h>

#include "dict.h"

int lookup(Dictrec * sought, const char * resource) {
	Dictrec dr;
	static FILE * in;
	static int first_time = 1;
		/* open up the file
		 *
		 * Fill in code. */
	 in = fopen(resource, "r");
	 strcat(sought -> word, "\n");

	/* read from top of file, looking for match
	 *
	 * Fill in code. */

	while(!feof(in)){
		fread(&dr, sizeof(Dictrec), 1, in);

		if(strcmp(dr.word, sought->word) == 0){
			strcpy(sought -> text, dr.text);

			return FOUND;
		}
	}
	fclose(in);

	return NOTFOUND;
}

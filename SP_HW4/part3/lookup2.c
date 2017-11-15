/*
 * lookup2 : local file ; setup an in-memory index of words
 *             and pointers into the file. resource is file name
 *             use qsort & bsearch
 */

#include <stdlib.h>
#include <string.h>
#include "dict.h"

typedef struct {
    char word[WORD];  /* The word to be looked up */
    long off;         /* Offset into file for word definition */
} Index;

/*
 * This ugly little function can be used by qsort & bsearch
 * It compares the word part of two structures of type Index
 */

int dict_cmp(const void *a,const void *b) {
	return strcmp(((Index *)a)->word,((Index *)b)->word);
}

int lookup(Dictrec * sought, const char * resource) {
	static Index * table;
	Dictrec dr;
	static int numrec;
	int i;
	Index * found, tmp;
	static FILE * in;
	static int first_time = 1;
  long fileSize;

	if (first_time) { /* set up index */
		first_time = 0;

		/* Open file.
		 *
		 * Fill in code. */
     in = fopen(resource, "r");
     if(!in){
       perror("fopen()");
       exit(1);
     }
		/* Get number records in file by dividing ending file offset by recsize.
		 *
		 * Fill in code. */
     fseek(in, 0, SEEK_END);
     fileSize = ftell(in);
     numrec = fileSize / sizeof(Dictrec);
		/* Go to the beginning of the file.
		 *
		 * Fill in code. */
     fseek(in, 0, SEEK_SET);

		/* Allocate zeroed-out memory: numrec elements of struct Index. */
		 table = calloc(numrec, sizeof(Index));

		/* Read the file into the just-allocated array in memory.
		 *
		 * Fill in code. */
    int j = 0;
    while(!feof(in)){
		    fread(&dr, sizeof(Dictrec), 1, in);
        strcpy(tmp.word, dr.word);
        tmp.off = ftell(in) - sizeof(Dictrec);

        table[j] = tmp;
        j++;
	  }

    // for(i = 0; i < numrec; i++){
    //   printf("word: %s\n", table[i].word);
    //   printf("off: %ld\n", table[i].off);
    // }

		/* Sort the table of entry/offset Index structures. */
		 qsort(table,numrec,sizeof(Index),dict_cmp);

	} /* end of first-time initialization */

	/* use bsearch to find word in the table; seek & read from file if found. */
	strcpy(tmp.word,sought->word);
	found = bsearch(&tmp,table,numrec,sizeof(Index),dict_cmp);

	/* If found, go to that place in the file, and read the record into the
	 * caller-supplied space. */
	if (found) {
    fseek(in, (found -> off), SEEK_SET);
    fread(&dr, sizeof(Dictrec), 1, in);
    strcpy(sought -> text, dr.text);

		return FOUND;
	}
	return NOTFOUND;
}

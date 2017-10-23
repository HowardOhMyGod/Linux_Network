/*
 * convert.c : take a file in the form
 *  word1
 *  multiline definition of word1
 *  stretching over several lines,
 * followed by a blank line
 * word2....etc
 * convert into a file of fixed-length records
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "dict.h"
#define BIGLINE 512

int main(int argc, char **argv) {
	FILE *in;
	FILE *out;        /* defaults */
	char line[BIGLINE];
	static Dictrec dr, blank;

	/* If args are supplied, argv[1] is for input, argv[2] for output */
	if (argc==3) {
		if ((in =fopen(argv[1],"r")) == NULL){DIE(argv[1]);}
		if ((out =fopen(argv[2],"w")) == NULL){DIE(argv[2]);}
	}
	else{
		printf("Usage: convert [input file] [output file].\n");
		return -1;
	}

	/* Main reading loop : read word first, then definition into dr */

	/* Loop through the whole file. */
  char defi[BIGLINE] = {0};
	while (fgets(line, BIGLINE, in)) {

		/* Create and fill in a new blank record.
		 * First get a word and put it in the word field, then get the definition
		 * and put it in the text field at the right offset.  Pad the unused chars
		 * in both fields with nulls.
		 */

		/* Read word and put in record.  Truncate at the end of the "word" field.
		 *
		 * Fill in code. */
    //  get word
     if(strlen(line) < 15 && strlen(line) != 1){
       strcpy(blank.word, line);
     } else if (strlen(line) != 1){ // get text
       strcat(defi, line);
     } else { //blank line indicate word end
         strcpy(blank.text, defi);
         printf("word: %s", blank.word);
         printf("Defi: %s", blank.text);
         printf("---------\n");

         memset(defi, 0, BIGLINE);

         for(int i = 0; i < WORD; ++i){
           if(i == WORD - 1){
             strcat(blank.word, "\0");
           } else if(!blank.word[i]){
             strcat(blank.word, " ");
           }
         }

         for(int i = 0; i < TEXT; ++i){
           if(i == TEXT - 1){
             strcat(blank.text, "\0");
           } else if(!blank.text[i]){
             strcat(blank.text, " ");
           }
         }

         printf("Word Length: %ld\n", strlen(blank.word));
         printf("Defi Length: %ld\n", strlen(blank.text));

         fprintf(out, "%s\n%s\n", blank.word, blank.text);

     }
	}
  // last word and line
  strcpy(blank.text, line);

  for(int i = 0; i < WORD; i++){
    if(i == WORD - 1){
      strcat(blank.word, "\0");
    } else if(!blank.word[i]){
      strcat(blank.word, " ");
    }
  }

  for(int i = 0; i < TEXT; ++i){
    if(i == TEXT - 1){
      strcat(blank.text, "\0");
    } else if(!blank.text[i]){
      strcat(blank.text, " ");
    }
  }
  fprintf(out, "%s\n%s\n", blank.word, blank.text);

	fclose(in);
	fclose(out);
	return 0;
}

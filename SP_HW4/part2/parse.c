/*
* parse.c : use whitespace to tokenise a line
* Initialise a vector big enough
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell.h"

/* Parse a commandline string into an argv array. */
char ** parse(char *line) {

  static char delim[] = " \t\n"; /* SPACE or TAB or NL */
  int count = 0;
  char * token;
  char **newArgv;

  /* Nothing entered. */
  if (line == NULL) {
    return NULL;
  }
  /* Init strtok with commandline, then get first token.
   * Return NULL if no tokens in line.
 *
 * Fill in code.
   */
  token = strtok(line, delim);

  if(token == NULL){
    return NULL;
  }

  /* Create array with room for first token.
   *
 * Fill in code.
 */

 newArgv = malloc(2*sizeof(char *));
 newArgv[count] = malloc((strlen(token) + 1)*sizeof(char));
//  printf("Token store in %p, value = %s\n", newArgv[count], newArgv[count]);
 strcpy(newArgv[count], token);
//  printf("Token store in %p, value = %s\n", token, token);
//  newArgv[count] = temp;
//  printf("Token store in %p, value = %s\n", newArgv[count], newArgv[count]);

 // printf("[%d] : %s \n",count, newArgv[count]);
 count++;
 token = strtok(NULL, delim);

  /* While there are more tokens...
 *
 *  - Get next token.
 *	- Resize array.
 *  - Give token its own memory, then install it.
 *
   * Fill in code.
 */
 while(token != NULL){
   newArgv = realloc(newArgv, (count + 2) * sizeof(char *));
   newArgv[count] = malloc((strlen(token) + 1)*sizeof(char));
  //  printf("Token store in %p, value = %s\n", newArgv[count], newArgv[count]);
  //  printf("Token len = %zu\n", (strlen(token) + 1)*sizeof(char));
   strcpy(newArgv[count] , token);
  //  printf("Token store in %p, value = %s\n", token, token);

  //  newArgv[count] = temp;
  //  printf("Token store in %p, value = %s\n", newArgv[count], newArgv[count]);
  //  printf("[%d] : %s \n",count, newArgv[count]);
   token = strtok(NULL, delim);
   count++;
 }

  /* Null terminate the array and return it.

 */
//  printf("count = %d", count);
  // temp = malloc(3);
  // strcpy(temp, '\0');
  newArgv[count] = NULL;
  // printf("[%d]: %s", count,newArgv[count]);
  return newArgv;
}


/*
* Free memory associated with argv array passed in.
* Argv array is assumed created with parse() above.
*/
void free_argv(char **oldArgv) {
/* Free each string hanging off the array.
 * Free the oldArgv array itself.
 *
 * Fill in code.
 */
 int i;
//  printf("Free\n");

 for(i = 0; oldArgv[i] != NULL; i++){
    //  printf("Free %p, value = %s\n", oldArgv[i], oldArgv[i]);
     free(oldArgv[i]);
 }
 free(oldArgv);
// printf("Free Array %p\n", oldArgv);
}

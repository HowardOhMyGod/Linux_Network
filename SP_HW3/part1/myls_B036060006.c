#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>

void list_directory( char *dir_path, char *options);
int isDirExist(char *dirname);
int isDash(char *argvs);
int isOption(char *argv, char option);

int main( int argc, char **argv) {
	if(argc > 2){
		printf("Usage: ./myls [-options]\n");
		exit(1);

	// input ./myls
	} else if (argc == 1){
			list_directory(NULL, NULL);
	// input ./myls [options]
	} else {

		if(isDash(argv[1])){ // input options
			printf("Find dash.\n");
			list_directory(NULL, argv[1]);

		} else if (isDirExist(argv[1])){ // specified directory
			printf("Find Dir.\n");
			list_directory(argv[1], NULL);

		} else {
			printf("Can't find directory or commands : %s\n", argv[1]);
			exit(1);
		}
	}

	return 0;
}

int isDash(char *argv){
	if(argv[0] == '-') return 1;
	else return 0;
}

int isOption(char *argv, char option){
		if(argv == NULL) return 0;

		for(int i = 0; argv[i]; i++){
			if(argv[i] == option) return 1;
		}

		return 0;
}

int isDirExist(char *dirname){
	if(chdir(dirname) == 0) return 1;
	else return 0;
}

void list_directory( char *dir_path, char *options) {
	DIR *dirp;
	struct dirent *dent;
	struct stat buf;
	char path[1024];


	// get current directory path
	if(getcwd(path, sizeof(path)) != NULL){
		printf("%s:\n", path);
	} else {
		perror("getcwd()");
	}

	//open dir
	if((dirp = opendir(path)) == NULL){
		perror("opendir()");
		exit(1);
	}

	// print dir contents

	while(dent = readdir(dirp)){

		// exclude .. and .
		if(strcmp(dent -> d_name, ".") != 0 && strcmp(dent -> d_name, "..") != 0){

			// if F option print file type
			if(isOption(options, 'F') == 1){
				stat(dent -> d_name, &buf);

				if(S_ISDIR(buf.st_mode)){
					printf("%s/\n", dent -> d_name);

				} else if (buf.st_mode & S_IXUSR){
					printf("%s*\n", dent -> d_name);
				} else {
					printf("%s\n", dent -> d_name);
				}

			// normal print
			} else {
				printf("%s\n", dent -> d_name);
			}
		}
	}

	// close dir
	(void)closedir(dirp);

}

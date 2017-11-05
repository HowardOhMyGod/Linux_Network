#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>


int main( int argc, char* argv[] ) {
	int result;
	char ask = 'Y';
	DIR *dirp;
	struct dirent *dent;
	struct stat buf;

	if(argc != 3) {
		perror("./mymv needs to inputs.\n");
		exit(1);
	}

	//checkc if second argument exist
	if((dirp = opendir(".")) == NULL){
		perror("opendir()");
		exit(1);
	}

	// check if second argument is dir
	stat(argv[2], &buf);
	if(S_ISDIR(buf.st_mode)){
		char dir_path[120];

		strcat(dir_path, "./");
		strcat(dir_path, argv[2]);
		strcat(dir_path, "/");
		strcat(dir_path, argv[1]);

		strcpy(argv[2], dir_path);
	}

	// check if second argument exist
	while(dent = readdir(dirp)){
		if(strcmp(dent -> d_name, argv[2]) == 0){
			printf("mymv: overwirte '%s' ? (Y/N)", argv[2]);
			scanf(" %c", &ask);
			break;
		}
	}

	if(ask == 'Y'){
		if(result = rename(argv[1], argv[2]) < 0){
			perror("rename()");
			exit(1);
		}
	}


	return 0;
}

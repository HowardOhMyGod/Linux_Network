#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>


void list_directory( char *dir_path, int f_flag, int r_flag);
void listAll(char *dir_path, int f_flag);

int main( int argc, char **argv) {
	int c, f_flag = 0, r_flag = 0;
	int index;
	int nonOptCount = 0;

	if(argc > 2){
		printf("Usage: ./myls [-options]\n");
		exit(1);

	// input ./myls
	} else if (argc == 1){
			list_directory(NULL, 0, 0);
	// input ./myls [options]
	} else {
		//get otions
		while((c = getopt(argc, argv, "RF")) != -1){
			switch(c){

				case 'F':
					f_flag = 1;
					break;

				case 'R':
					r_flag = 1;
					break;
				default:
					break;
			}
		}

		// printf("f_flag = %d, r_flag = %d\n", f_flag, r_flag);

		for(index = optind; index < argc; index++){
			nonOptCount ++;
		}

		// specified directory
		if(r_flag == 0 && f_flag == 0 && nonOptCount == 1){
			list_directory(argv[1], 0, 0);

		// current directory with options
	} else if (r_flag != 0) {
			listAll(".", f_flag);
		} else {
			list_directory(NULL, f_flag, r_flag);
		}

	}
	return 0;
}

void list_directory( char *dir_path, int f_flag, int r_flag){
	char path[100];
	DIR *dirp;
	struct dirent *dent;
	struct stat buf;

	if(dir_path == NULL){
		if(getcwd(path, sizeof(path)) == NULL){
			perror("getcwd()");
			exit(1);
		}
	} else {
		if(chdir(dir_path) == -1){
			perror("getcwd()");
			exit(1);
		}

		if(getcwd(path, sizeof(path)) == NULL){
			perror("getcwd()");
			exit(1);
		}

	}

	printf("%s:\n", path);

	//open dir
	if((dirp = opendir(path)) == NULL){
		perror("opendir()");
		exit(1);
	}

	while(dent = readdir(dirp)){
		//exclude . and ..
		if(strcmp(dent -> d_name, ".") != 0 && strcmp(dent -> d_name, "..")){
			if(f_flag == 1){
				stat(dent -> d_name, &buf);

				if(S_ISDIR(buf.st_mode)) printf("%s/\n", dent -> d_name);
				else if (buf.st_mode & S_IXUSR) printf("%s*\n", dent -> d_name);
				else printf("%s\n", dent -> d_name);
			} else {
				printf("%s\n", dent -> d_name);
			}
		}
	}

	(void)closedir(dirp);

}

void listAll(char *dir_path, int f_flag){
	char path[100];
	DIR *dirp;
	struct dirent *dent;
	struct stat buf;
	char *subdir;

	if((dirp = opendir(dir_path)) == NULL){
		// perror("opendir()");
		return;
	}

	chdir(dir_path);

	if(getcwd(path, sizeof(path)) == NULL){
		perror("getcwd()");
		exit(1);
	}

	printf("\n%s : \n", path);

	while((dent = readdir(dirp))){

		stat(dent -> d_name, &buf);

		if(S_ISDIR(buf.st_mode)){
			if(strcmp(dent -> d_name, ".") == 0 || strcmp(dent -> d_name, "..") == 0) continue;


			subdir = malloc(strlen(path) + strlen(dent -> d_name) + 2);

			strcpy(subdir, path);
			strcat(subdir, "/");
			strcat(subdir, dent -> d_name);

			if(f_flag) printf("%s/\n", dent -> d_name);
			else printf("%s\n", dent -> d_name);

			listAll(subdir, f_flag);
		} else {
			if(f_flag && buf.st_mode & S_IXUSR) printf("%s*\n", dent -> d_name);
			else printf("%s\n", dent -> d_name);
		}
	}

	(void)closedir(dirp);
}

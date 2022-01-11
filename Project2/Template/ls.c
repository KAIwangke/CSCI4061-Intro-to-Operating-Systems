#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
//#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

void ls(char *path, bool recurse_flag) {
    // if the given path is NULL, set it to the current directory
    if(path == NULL) {path = ".";}
    DIR *dir;
    struct dirent *dirent_ptr = NULL;
    
    // if the path cannot be opened, return error
    if((dir = opendir(path)) == NULL){
        printf("errno = %d : %s\n", errno, strerror(errno));
        exit(errno);
    }
 
    while((dirent_ptr = readdir(dir)) != NULL){
        // omit . and ..
        if(strncmp(dirent_ptr->d_name, ".", 1) != 0 || strncmp(dirent_ptr->d_name, "..", 1) != 0){
        // if the current reading entry is a directory
          if(dirent_ptr->d_type == DT_DIR){
            printf("Directory: ");
            printf("%s\n", dirent_ptr->d_name);
            // enable recursion
            if(recurse_flag == true){
	      char new_path[100];
	      strcpy(new_path, path);
	      strcat(new_path, "/");
	      strcat(new_path, dirent_ptr->d_name);
              ls(new_path, recurse_flag);
            }
          }
          else{ printf("%s\n", dirent_ptr->d_name);}       
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]){
	if(argc < 2){ // No -R flag and no path name
		ls(NULL, false);
	} else if(strcmp(argv[1], "-R") == 0) { 
		if(argc == 2) { // only -R flag
			ls(NULL, true);
		} else { // -R flag with some path name
			ls(argv[2], true);
		}
	}else { // no -R flag but path name is given
    	ls(argv[1], false);
  }
	return 0;
}

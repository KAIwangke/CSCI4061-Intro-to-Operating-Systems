#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>


void printStats(char* path) {

	
	DIR* dir;
	// TODO: open the directory
	dir=opendir(path);
	struct dirent* entry;
    if (dir == NULL) {
        printf("Cannot open current directory");
    }
	if (ENOENT != errno) {
	while ((entry = readdir(dir)) != NULL ) {

		// TODO: continue if d_name is . or ..
        
		if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name,"..")==0)continue;
		
		struct stat* buf = (struct stat*)malloc(sizeof(struct stat));
		// TODO: get file attributes (try to think of which function should be used, stat() or lstat()?)
		//lstat() is identical to stat(), 
		//except that if path is a symbolic link, t
		//hen the link itself is stat-ed, not the file that it refers to.
		lstat(entry->d_name,buf);
	
		printf("File: %s\n\tOwner: %d\n\tSize: %f\n\tisLink: %s\n",
			entry->d_name, buf->st_uid, (double) buf->st_size, (S_ISLNK(buf->st_mode))? "True": "False");
		// TODO: free the allocated space
        
	    free(buf);
	}
	closedir(dir);
	}
}

int main(int argc, char** argv) {

	if (argc < 2) {

		printf("Please provide the path!");
		exit(1);
	}

	printStats(argv[1]);
}

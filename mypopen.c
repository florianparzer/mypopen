/*
 ============================================================================
 Name        : mypopen.c
 Author      : Parzer Florian
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

static int procID = -1;
static bool isOpend = false;
static FILE* filestream;

FILE* mypopen(const char* cmd, const char *mode){
	if(isOpend == true){
		return NULL;
	}
	int pipefd[2];
	if(pipe(pipefd) != 0){
		perror("pipe");
		return NULL;
	}
	int pid;
	if((pid = fork()) < 0){
		//if(errno == )
		perror("Fork");
		return NULL;
	}
	if(pid != 0){

		//parent-process
		FILE* fd;
		//if(mode == "r"){
		if(strcmp(mode, "r") == 0){
			//Close Writing End
			while((close(pipefd[1])) == -1){
				perror("close");
				if(errno != EINTR){
					return NULL;
				}
			}
			fd = fdopen(pipefd[0], "r");
		}else{
			//Close Reading End
			while((close(pipefd[0])) == -1){
				perror("close");
				if(errno != EINTR){
					return NULL;
				}
			}
			fd = fdopen(pipefd[1], "w");
		}
		if(fd == NULL){
			perror("fdopen");
			return NULL;
		}
		procID = pid;
		filestream = fd;
		isOpend = true;
		return fd;


	}else if(pid == 0){

		//child process
		if(strcmp(mode, "r") == 0){
			//Close Reading end of Child Process
			while((close(pipefd[0])) == -1){
				perror("close child);");
				if(errno != EINTR){
					return NULL;
				}
			}

			//Assign writing end to STOUT
			while(dup2(pipefd[1], STDOUT_FILENO) == -1){
				perror("dup2");
				if(errno != EINTR){
					return NULL;
				}
			}

			//close the original writing end
			while((close(pipefd[1])) == -1){
				perror("close child");
				if(errno != EINTR){
					return NULL;
				}
			}
		}else{
			//CLose Writing end of Child process
			while((close(pipefd[1])) == -1){
				perror("close child");
				if(errno != EINTR){
					return NULL;
				}
			}
			//Assign reading end to STDIN
			while(dup2(pipefd[0], STDIN_FILENO) == -1){
				perror("dup2");
				if(errno != EINTR){
					return NULL;
				}
			}

			//close the original reading end
			while((close(pipefd[0])) == -1){
				perror("close child");
				if(errno != EINTR){
					return NULL;
				}
			}
		}
		execlp("/bin/sh", "sh", "-c", cmd);
		perror("execlp");

	}
	return NULL;
}

int mypclose(FILE* stream){
	pid_t pid;
	if(isOpend != true){
		errno = EINVAL;
		return -1;
	}

	if(filestream != stream){
		errno = EINVAL;
		return -1;
	}

	if(stream == NULL){
		errno = EINVAL;
		return -1;
	}
	if(fclose(stream) != 0){
		if(kill(procID, 1) == 0){
			while((pid = waitpid(-1, NULL, 0)) == procID){
				errno = EBADF;
			}
			return -1;
		}
		perror("fclose");
		return -1;
	}

	while((pid = waitpid(procID, NULL, 0)) != procID){
		if(pid == -1){
			perror("weitpid");
			return -1;
		}
	}
	reset();
	return 0;
}


void reset(){
	procID = -1;
	filestream = NULL;
	isOpend = false;
}

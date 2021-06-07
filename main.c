/*#include "mypopen.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char cmd[20] = "ls /home/osboxes";
	char buff[100];
	FILE* fdO = popen(&cmd, "r");



	fgets(buff, sizeof(buff), fdO);

	printf("%s\n", buff);
	pclose(fdO);
	FILE* fdP = mypopen(&cmd, "r");
	//printf("Mine\n");
	fgets(buff, sizeof(buff), fdP);
	printf("%s", buff);
	fgets(buff, sizeof(buff), fdP);
	printf("%s", buff);
	mypclose(fdP);
}

*/

#include "mypopen.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	char cmd[20] = "ls -la /home/osboxes";
	char buff[100];
	//char[] mode = "r";
	printf("Test Read\nPopen:\n");
	FILE* fdO = popen(&cmd, "r");

	while((fgets(buff, sizeof(buff), fdO)) != NULL){
		printf("%s", buff);
	}


	pclose(fdO);

	printf("\nmyPopen:\n");

	FILE* fdP = mypopen(&cmd, "r");

	while((fgets(buff, sizeof(buff), fdP)) != NULL){
		printf("%s", buff);
	}

	mypclose(fdP);

	printf("\nTest Write\nPopen:\n");
	strcpy(cmd, "grep test");
	fdO = popen(&cmd, "w");

	fprintf(fdO, "das ist ein test\n ok oder nicht\ntest test");
	pclose(fdO);

	printf("myPopen:\n");
	fdP = mypopen(&cmd, "w");

	fprintf(fdP, "das ist ein test\n ok oder nicht\ntest test");
	mypclose(fdP);
	return 0;
}

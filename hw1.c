/***
	Vulnerable C program
***/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <string.h>

int getShAddr() {
	char *handle = (char *)dlopen("libc.so.6", 1);
	//printf("libc addr: %08x\n", handle);
	int i = 0;
	while (1) {
		if (handle[i] == 0x73 && handle[i + 1] == 0x68 && handle[i + 2] == 0) {
			return (int)(handle + i);
		}
		++i;
	}
}

int getSysAddr() {
	char *handle = (char *)dlopen("libc.so.6", 1);
	int sys_addr = (char *)dlsym(handle, "system");
	return sys_addr;
}

FILE *getReader(char *filename) {
	char fn[50] = "\0";
	strcat(fn + strlen(fn), filename); //<--stack buffer overflow
	return fopen(fn, "rb");
}

FILE *getWriter(char *filename) {
	char fn[50] = "\0";
	strcat(fn + strlen(fn), filename);
	return fopen(fn, "wb");
}

int main(int argc, char **argv) {
	/***
		For your convinience, the addresses of "sh" and system() are printed below
	***/
	printf("\"sh\" address: %08x\n", (unsigned int)getShAddr());
	printf("system() address: %08x\n", (unsigned int)getSysAddr());
	FILE* sourceFile;
	FILE* destFile;
	char buf[50];
	int numBytes;
	if (argc!=3) {
		printf("Usage: fcopy source destination\n");
		return 1;
	}
	sourceFile = getReader(argv[1]);
	destFile = getWriter(argv[2]);
	if (sourceFile==NULL) {
		printf("Could not open source file\n");
		return 2;
	}
	if (destFile==NULL) {
		printf("Could not open destination file\n");
		return 3;
	}
	while (numBytes = fread(buf, 1, 50, sourceFile)) {
		fwrite(buf, 1, numBytes, destFile);
	}
	fclose(sourceFile);
	fclose(destFile);
	return 0;
}

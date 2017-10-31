#include <stdint.h>
#include <textDriver.h>
#include <syscallDispatcher.h>
#include <keyboard.h>

static const syscall syscalls[]={0, 0, 0, read, write};


uint64_t syscallDispatcher(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx){
	
	return syscalls[rax](rbx, rcx, rdx);
}





uint64_t write(uint64_t fd, uint64_t buffer, uint64_t count){
	int i = 0;
	/*salida estandar*/
	if (fd == STDOUT){
		for(i=0; i < count ; i++){
			putChar(((char*)buffer)[i]);
		}
	}
	return i;
}

uint64_t read(uint64_t fd, uint64_t buffer, uint64_t count){
	int i;
		/*entrada estandar*/
	if(fd == STDIN){
		for(i=0; i < count ; i++){
			*(char*)buffer = consumeBuffer();
			buffer++;
		}
	}
	return i;
}
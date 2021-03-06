#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include "kernelThread.h"

#define MAX_QTY_PROCESSES 512

#define NEW 0
#define READY 1
#define RUNNING 2
#define WAITING 3
#define TERMINATED 4
#define WAITINGPROCESS 5

typedef struct {
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;
} stack;



typedef struct PCB {
	uint32_t pid;
	uint32_t parentPid;
	TCB threads[MAX_QTY_THREADS];
	uint32_t numberOfThreads;
	uint32_t idCounter;
} PCB;

typedef struct processInfo {
	uint32_t pid;
	uint64_t sizeAllocated;
	uint8_t state;
	int priority;
} processInfo;

typedef struct processesInfoTable {
	uint32_t numberOfProcessesOnTable;
	processInfo *list;
} processesInfoTable;

typedef struct processTable {
	PCB list[MAX_QTY_PROCESSES];
	uint32_t numberOfProcessesOnTable;
	uint32_t pidCounter;
} processTable;



void initializeProcessTable();
uint32_t startNewProcess(uint64_t rip, int argc, char **argv);
stack *initializeStack(uint64_t rsp, uint64_t rip, int argc, char **argv);
PCB *addNewProcessToTable(uint64_t rip, int argc, char **argv);
uint32_t getNextPid();
PCB *getCurrentProcess();
void setCurrentProcessState(int state);
void setCurrentProcess(PCB *process);
void initializeProcessLog();
uint32_t getCurrentProcessPID();
void fillProcessesInfo(processesInfoTable *processes);


PCB *getProcessPCB(int pid);

#endif

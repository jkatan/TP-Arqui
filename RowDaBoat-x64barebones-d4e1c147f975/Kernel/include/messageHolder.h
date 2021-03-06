#ifndef MESSAGE_HOLDER_H
#define MESSAGE_HOLDER_H

#define MAX_MESSAGE_SIZE 200

#include "memoryManager.h"
#include "mutex.h"
#include <lib.h>
#include <stddef.h>
#include <stdint.h>

typedef struct messageHolder {
	uint32_t senderPID;
	uint32_t receiverPID;
	uint32_t senderThreadID;
	uint32_t receiverThreadID;
	char *id;
	char message[MAX_MESSAGE_SIZE];
	int currentMessageIndex;
	mutex *messageMutex;
} messageHolder;

typedef struct messageHolderNode {
	messageHolder *data;
	struct messageHolderNode *next;
} messageHolderNode;

typedef struct messageHolderLinkedList {
	messageHolderNode *first;
} messageHolderLinkedList;

messageHolder *createMessageHolder(char *id);
messageHolder *retrieveMessageHolder(char *id);
void destroyMessageHolder(char *id);
void sendMessage(messageHolder *message, char *data, int size);
void receiveMessage(messageHolder *message, char *storageBuffer, int size);

#endif

#include "videoDriver.h"
#include <messageHolder.h>
#include <process.h>
#include "kernelThread.h"

messageHolderLinkedList messageHolderList;
char messageMutexIdCounter;

void sendMessage(messageHolder *message, char *data, int size)
{
	lock(message->messageMutex);

	message->senderPID = getCurrentProcessPID();
	message->senderThreadID = getCurrentThreadID();

	int i;

	for (i = 0; i < size; i++) {
		if (message->currentMessageIndex == MAX_MESSAGE_SIZE) {
			setCurrentThreadState(WAITING);
			while (getCurrentThread()->state == WAITING)
				; // Espera a que lo despierte el proceso
				  // receiver
		}

		message->message[message->currentMessageIndex] = *(data + i);
		message->currentMessageIndex++;
	}

	if (message->receiverPID != -1 &&
	    getThreadTCB(message->receiverPID, message->receiverThreadID)->state == WAITING) {
		stopThreadWait(getThreadTCB(message->receiverPID, message->receiverThreadID));
	}
	unlock(message->messageMutex);
}

void receiveMessage(messageHolder *message, char *storageBuffer, int size)
{
	lock(message->messageMutex);

	message->receiverPID = getCurrentProcessPID();
	message->receiverThreadID = getCurrentThreadID();
	int i;

	for (i = 0; i <= size; i++) {
		if (message->currentMessageIndex - i < 0) {
			message->currentMessageIndex -= i;
			setCurrentThreadState(WAITING);
			while (getCurrentThread()->state == WAITING)
				; // Bloqueo hasta que lo despierte el proceso
				  // sender
		}

		storageBuffer[i] =
		    message->message[message->currentMessageIndex - size + i];
	}

	message->currentMessageIndex -= i;

	if (message->senderPID != -1 &&
	    getThreadTCB(message->senderPID, message->senderThreadID)->state == WAITING) {
		stopThreadWait(getThreadTCB(message->senderPID, message->senderThreadID));
	}

	unlock(message->messageMutex);
}

void destroyMessageHolder(char *id)
{
	if (id == NULL) {
		return;
	}

	messageHolderNode *currentMessage = messageHolderList.first;
	messageHolderNode *previousMessage = NULL;

	while (currentMessage != NULL) {
		if (strcmp(currentMessage->data->id, id) == 0) {

			if (previousMessage ==
			    NULL) // Current es el primer elemento de la lista
			{
				messageHolderList.first = currentMessage->next;
				deallocate(currentMessage->data);
				deallocate(currentMessage);
				destroyMutex(id);
				return;
			}

			previousMessage->next = currentMessage->next;
			deallocate(currentMessage->data);
			deallocate(currentMessage);
			destroyMutex(id);
			return;
		}

		previousMessage = currentMessage;
		currentMessage = currentMessage->next;
	}
}

messageHolder *retrieveMessageHolder(char *id)
{
	if (id == NULL) {
		return NULL;
	}

	messageHolderNode *currentMessage = messageHolderList.first;

	while (currentMessage != NULL) {
		if (strcmp(currentMessage->data->id, id) == 0) {
			return currentMessage->data;
		}

		currentMessage = currentMessage->next;
	}

	return NULL;
}

messageHolder *createMessageHolder(char *id)
{
	if (id == NULL) {
		return NULL;
	}

	mutex *messageMutexToAdd;
	messageHolder *messageHolderToAdd;
	messageHolderNode *messageHolderNodeToAdd;
	messageHolderNode *currentMessage = messageHolderList.first;
	messageHolderNode *previousMessage = NULL;

	while (currentMessage != NULL) {
		if (strcmp(currentMessage->data->id, id) == 0) {
			return NULL;
		}

		previousMessage = currentMessage;
		currentMessage = currentMessage->next;
	}

	messageHolderToAdd = allocate(1 * sizeof(messageHolder));
	messageHolderNodeToAdd = allocate(1 * sizeof(messageHolderNode));
	messageMutexToAdd = createMutex(id);

	messageHolderToAdd->id = id;
	messageHolderToAdd->messageMutex = messageMutexToAdd;
	messageHolderToAdd->senderPID = -1;
	messageHolderToAdd->receiverPID = -1;
	messageHolderNodeToAdd->data = messageHolderToAdd;

	messageMutexIdCounter++;

	if (previousMessage == NULL) {
		messageHolderList.first = messageHolderNodeToAdd;
	} else {
		previousMessage->next = messageHolderNodeToAdd;
	}

	return messageHolderToAdd;
}

#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 1024

struct sharedMemoryBuffer
{
    sem_t producer;
    sem_t consumer;
    size_t bufSize;
    char buffer[BUF_SIZE];  
};


void ErrorReturned(char* msg);
int CreateSizeSharedMem(char* shmName);
void* MappingMemory(int fileDescriptor, struct sharedMemoryBuffer *smb);







/* Listening programe */
#include <ctype.h>
#include "header.h"

int main()
{
    char* shmName = "mySharedMemory";
    //if already exist, delete it
    shm_unlink(shmName);

    int fd = CreateSizeSharedMem(shmName);

    struct sharedMemoryBuffer *smb;
    smb = MappingMemory(fd, smb);

// Initializing semaphores

    if (sem_init(&smb->producer, 1, 0) == -1)
        ErrorReturned("Error in sem init - producer:");

    if (sem_init(&smb->consumer, 1, 0) == -1)
        ErrorReturned("Error in sem init - consumer:");



    while (1)
    {
        puts("Waiting on consumer ...");

        if (sem_wait(&smb->consumer) == -1)
        {
            ErrorReturned("Error in consumer sem:");
        }

        puts("Doing my job ...");

        printf("\n\nMessage from sender in bounce: %s\n\n", smb->buffer);

        puts("Finished my job ...");

        puts("Posting for producer");
        if (sem_post(&smb->producer) == -1)
        {
            ErrorReturned("Error in producer sem:");
        }

        puts("Posted on producer");
    }
    
    shm_unlink(shmName);
    return 0;
}
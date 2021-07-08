#include "temp_fcns.h"

int main()
{
    shm_unlink(SHARED_MEM_NAME);
    int fd = CreateSizeSharedMem(SHARED_MEM_NAME);

    struct sharedMemoryBuffer* smb;
    smb = MappingMemory(fd, smb);

    //Semaphores
    if (sem_init(&smb->producer, 1, 0) == -1)
        error("Error in sem init - producer:");

    if (sem_init(&smb->consumer, 1, 0) == -1)
        error("Error in sem init - consumer:");

    puts("Waiting for client");
    
    while (1)
    {
        if (sem_wait(&smb->consumer) == -1)
        {
            error("Error in consumer sem:");
        }

        printf("%s\n", smb->buffer);

        if (sem_post(&smb->producer) == -1)
        {
            error("Error in producer sem:");
        }
    }

    shm_unlink(SHARED_MEM_NAME);
    return 0;
}
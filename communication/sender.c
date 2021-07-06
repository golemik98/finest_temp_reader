#include <string.h>
#include "header.h"

int main()
{
    char* shmName =  "mySharedMemory";
    char* message = "Sender: My message to bounce\n";
    int messageLength = strlen(message);

    if (messageLength > BUF_SIZE)
    {
        fprintf(stderr, "Message too long\n");
        exit(EXIT_FAILURE);
    }
    
    // Now I open existing shared memory object
    // Then i need to map it into this address space

    int fd = shm_open(shmName, O_RDWR, 0);
    if (fd == -1)
        ErrorReturned("Errir in opening existing shared memory:");

    struct sharedMemoryBuffer *smb;
    smb = MappingMemory(fd, smb);
    if (smb == MAP_FAILED)
        ErrorReturned("Erro in mapping: ");

    //Copying data into the shm obj

    smb->bufSize = messageLength;
    memcpy(&smb->buffer, message, messageLength);

    // Tell out peer that it can now acces shared memory


    while(1)
    {
        puts("Posting on consumer ...");

        if (sem_post(&smb->consumer) == -1)
            ErrorReturned("Error in sem producer posting: ");

        puts("Posted on consumer");

        //Wait until bounce finish his task
        puts("Waiting for bounce ...");


        puts("Waiting for producer ...");
        if (sem_wait(&smb->producer) == -1)
            ErrorReturned("Error in sem consumer posting: ");
        
        puts("Done");
        sleep(1);
    }
    
    return 0;
}
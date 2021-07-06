#include "header.h"

void ErrorReturned(char* msg)
{
    perror(msg);
    exit(1);
}

int CreateSizeSharedMem(char* shmName)
{
    int fd = 0;
    fd = shm_open(shmName, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1)
        ErrorReturned("Error in shm_open:");

    if ( ftruncate(fd, sizeof(struct sharedMemoryBuffer)) == -1)
        ErrorReturned("Error in ftruncate:");

    return fd;
}

void* MappingMemory(int fileDescriptor, struct sharedMemoryBuffer *smb)
{
    smb = mmap(NULL, sizeof(*smb), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    
    if (smb == MAP_FAILED)
        ErrorReturned("Error in mapping memory:");

    return smb;
}


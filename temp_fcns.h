#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>

#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAIN_DIR_PATH "/sys/devices/w1_bus_master1/"
#define TEMP_FILE "/w1_slave"
#define BUF_SIZE 100
#define SHARED_MEM_NAME "MySharedMemory"

struct sharedMemoryBuffer
{
    sem_t producer;
    sem_t consumer;
    size_t bufSize;
    char buffer[BUF_SIZE];  
};

struct dataStructure
{
    DIR* dir;
    struct dirent* dirEntry;
    struct sharedMemoryBuffer* smb;
};


void error(const char * msg);
void Temp_reader(DIR* dir, struct dirent* dirEntry, struct sharedMemoryBuffer* smb);
void handler_timer(union sigval arg);




void ErrorReturned(char* msg);
int CreateSizeSharedMem(char* shmName);
void* MappingMemory(int fileDescriptor, struct sharedMemoryBuffer *smb);
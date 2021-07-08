#include "temp_fcns.h"

void error(const char * msg)
{
  perror(msg);
  puts("\n");
  exit(EXIT_FAILURE);
}


char* Timestamp()
{
    time_t ltime; /* calendar time */
    ltime=time(NULL); /* get current cal time */
    return asctime(localtime(&ltime));
}


void Temp_reader(DIR* dir, struct dirent* dirEntry, struct sharedMemoryBuffer* smb)
{
    char* sensorName;

    dir = opendir(MAIN_DIR_PATH);
    if (dir == NULL)
    {
        error("ERROR in opening main directory");
    }

    while (dirEntry = readdir(dir))
    {
        if (dirEntry->d_type == DT_DIR)
        {
            if (strncmp(dirEntry->d_name, "28", strlen("28")) == 0)
            {
                sensorName = dirEntry->d_name;

                char* path = calloc(64, sizeof(char));
                strncat(path, MAIN_DIR_PATH, strlen(MAIN_DIR_PATH)); //add /sys/...
                strncat(path, sensorName, strlen(sensorName)); // add /sys/.../28-00000...
                strncat(path, "/w1_slave", strlen("/w1_slave"));           // add /sys/.../28-00000.../w1_slave

                int fd = fopen(path, "r");

                if (fd == NULL)
                {
                    error("ERROR in opening some w1_slave file");
                }

                // checking file size
                fseek(fd, 0, SEEK_END);
                int fileSize = ftell(fd);
                fseek(fd, 0, SEEK_SET);

                char* buffer = calloc(fileSize, sizeof(char));
                fread(buffer, sizeof(char), fileSize, fd);
                char* textCursor = strstr(buffer, "t=");

                if (textCursor != NULL) //gives NULL if string is not founded
                {
                    char message[100];    
                    memset(message, 0, sizeof(message));
                    
                    char* timeStampValue = Timestamp();
                    strlen(timeStampValue);

                    char temp[4];
                    textCursor += 2;
                    float temperature = atof(textCursor);
                    temperature = temperature / 1000;
                    strncat(message, timeStampValue, strlen(timeStampValue) - 1);
                    strncat(message, " : ", strlen(" : "));
                    strncat(message, sensorName, strlen(sensorName));
                    gcvt(temperature, 4, temp); //conv from float to string
                    strncat(message, " : ", strlen(" : "));
                    strncat(message, temp, strlen(temp));
                    // printf("%s : %.2f\n", sensorName, temperature);
                
                    memcpy(smb->buffer, message, sizeof(message));
                    
                    if (sem_post(&smb->consumer) == -1)
                        error("Error in sem producer posting: ");
                   
                    if (sem_wait(&smb->producer) == -1)
                        error("Error in sem consumer posting: ");
                }
                else
                {
                    printf("%s : disconnected\n", sensorName);
                }

                free(path);
                free(buffer);
                
                close(fd);
            }
        }

    }



}

void handler_timer(union sigval arg)
{
    long threadID;
    // threadID = pthread_self();
    // printf("Hello its %ld, just started my work\n", threadID);
    // puts("Working ...");
    struct dataStructure *args;
    args = arg.sival_ptr;
    
    
    Temp_reader(args->dir, args->dirEntry, args->smb);
    
    
    // char pause[21];
    // for (size_t i = 0; i < 20; i++)
    // {
    //     strncat(pause, "-", 1);    
    // }
    // strncat(pause, '\0', 1);
    
    // memcpy(args->smb->buffer, pause, sizeof(pause));


    puts("Data send to receiver");
}


int CreateSizeSharedMem(char* shmName)
{
    int fd = 0;
    fd = shm_open(shmName, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);

    if (fd == -1)
        error("Error in shm_open:");

    if ( ftruncate(fd, sizeof(struct sharedMemoryBuffer)) == -1)
        error("Error in ftruncate:");

    return fd;
}

void* MappingMemory(int fileDescriptor, struct sharedMemoryBuffer *smb)
{
    smb = mmap(NULL, sizeof(*smb), PROT_READ | PROT_WRITE, MAP_SHARED, fileDescriptor, 0);
    
    if (smb == MAP_FAILED)
        error("Error in mapping memory:");

    return smb;
}
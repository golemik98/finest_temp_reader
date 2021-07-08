#include "temp_fcns.h"
#include <mosquitto.h>

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

    //mqtt publisher
    struct mosquitto *mosq;
    mosquitto_lib_init();

    mosq = mosquitto_new("temp_reader", true, NULL); //cliend ID
    int rc = mosquitto_connect(mosq, "192.168.1.153", 1883, 60);

    if(rc != 0)
    {
        error("Error in connecting to the borker.");
        mosquitto_destroy(mosq);
        return -1;
    }

    puts("Connected to the broker.");

    int err = 0;

    while (1)
    {
        if (sem_wait(&smb->consumer) == -1)
        {
            error("Error in consumer sem:");
        }

        //printf("%s\n", smb->buffer);
        err = mosquitto_publish(mosq, NULL, "dev/temp", strlen(smb->buffer), smb->buffer, 0, false);

        if (err != MOSQ_ERR_SUCCESS)
        {
            error("Error in publishing: ");
        }
        puts("Data send to a broker");
        if (sem_post(&smb->producer) == -1)
        {
            error("Error in producer sem:");
        }
    }

    shm_unlink(SHARED_MEM_NAME);
    return 0;
}
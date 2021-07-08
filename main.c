#include "temp_fcns.h"


int main(){

    int sharedMemFd = shm_open(SHARED_MEM_NAME, O_RDWR, 0);
    
    if (sharedMemFd == -1)
    {
        error("Error in opening shared memory: ");
    }
    
    struct dataStructure arguments;


    arguments.smb = MappingMemory(sharedMemFd, arguments.smb);

    if (arguments.smb == MAP_FAILED)
        error("Erro in mapping: ");

    timer_t timerid;
    struct sigevent sev;

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = handler_timer;
    sev.sigev_value.sival_ptr = &arguments; //one or another
    sev.sigev_notify_attributes = NULL;

    timer_create(CLOCK_REALTIME, &sev, &timerid);

    printf("Timer created, timer ID is %#jx\n", (uintmax_t) timerid);
//STARTING TIMER
    struct itimerspec its; //timer specifications
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;

    its.it_interval.tv_sec = 5;
    its.it_interval.tv_nsec = 0;

    timer_settime(timerid, 0, &its, NULL);


    while (1)
    {
        continue;
    }
    

}
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <pthread.h>

void routine()
{
    puts("Thread said something");
}


void handler_thread(union sigval arg) //wrapper 
{
    //puts("Timer expired once, good bye");
    pthread_t th[4];

    int i;
    for (i = 0; i < 4; i++)
    {
        if (pthread_create(th + i, NULL, routine, NULL) != 0)
        {
            perror("ERROR in creating threads");
            return 1;
        }
        printf("Thread %ld has started\n", i);
    }
    

    for (i = 0; i < 4; i++)
    {
        if (pthread_join(th[i], NULL) != 0)
        {
            perror("ERROR in joiningthreads");
            return 1;
        }
        printf("Thread %ld has benn joined\n", i);
    }
}



int main()
{
//establish timer for threads
    timer_t timerid;
    struct sigevent sev;

    sev.sigev_notify = SIGEV_THREAD;
    sev.sigev_notify_function = handler_thread;
    sev.sigev_value.sival_ptr = NULL;
    sev.sigev_notify_attributes = NULL;


    timer_create(CLOCK_REALTIME, &sev, &timerid);

    printf("Timer created, timer ID is %#jx\n", (uintmax_t) timerid);

//STARTING TIMER
    struct itimerspec its; //timer specifications
    its.it_value.tv_sec = 1;
    its.it_value.tv_nsec = 0;

    its.it_interval.tv_sec = 4;
    its.it_interval.tv_nsec = 0;

    timer_settime(timerid, 0, &its, NULL);

    while (1)
    {
        puts("Main loop");
        sleep(1);
    }

    return 0; 
}
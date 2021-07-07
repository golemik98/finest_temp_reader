#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include <stdint.h>

#define MAIN_DIR_PATH "/sys/devices/w1_bus_master1/"
#define TEMP_FILE "/w1_slave"



void error(const char * msg)
{
  perror(msg);
  puts("\n");
  exit(EXIT_FAILURE);
}


void Temp_reader(DIR* dir, struct dirent* dirEntry)
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
                    textCursor += 2;
                    float temperature = atof(textCursor);
                    temperature = temperature / 1000;
                    printf("%s : %.2f\n", sensorName, temperature);
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

struct dataStructure
{
    DIR* dir;
    struct dirent* dirEntry;
};


void handler_timer(union sigval arg)
{
    long threadID;
    threadID = pthread_self();
    printf("Hello its %ld, just started my work\n", threadID);
    puts("Working ...");
    struct dataStructure *args;
    args = arg.sival_ptr;

    Temp_reader(args->dir, args->dirEntry);
    puts("DONE!");
}

int main(){


    struct dataStructure arguments;

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
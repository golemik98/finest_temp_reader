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


struct dataStructure
{
    DIR* dir;
    struct dirent* dirEntry;
};


void error(const char * msg);
void Temp_reader(DIR* dir, struct dirent* dirEntry);
void handler_timer(union sigval arg);
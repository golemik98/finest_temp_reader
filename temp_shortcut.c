#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

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

int main(){
    DIR* dir;
    struct dirent* dirEntry;

    while (1)
    {
        Temp_reader(dir, dirEntry);
        sleep(3);
    }
    

}
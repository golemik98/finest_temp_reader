#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

#define MAIN_DIR_PATH "/sys/devices/w1_bus_master1/"
#define TEMP_FILE "/w1_slave"

int allSensorAmountGlobal = 0;
int pluggedSensorsAmountGlobal = 0;

void error(const char * msg)
{
  perror(msg);
  puts("\n");
  exit(EXIT_FAILURE);
}

char** GetAllSensorsName(DIR* dir, struct dirent* dirEntry)
{
    int index = 0;

    dir = opendir(MAIN_DIR_PATH);
    if (dir == NULL)
    {
        error("ERROR in opening main directory");
    }
    

//Counting sensors
    while (dirEntry = readdir(dir))
    {
        if (dirEntry->d_type == DT_DIR)
        {
            if (strncmp(dirEntry->d_name, "28", strlen("28")) == 0)
            {
                index++;
            }
        }
    }
    closedir(dir);

    dir = opendir(MAIN_DIR_PATH); 

//Allocating space 

    char** allSensors = (char **)malloc(index * sizeof(char *));
    for (int i = 0; i < index; i++)
        allSensors[i] = (char *)malloc(20 * sizeof(char));

    memset(allSensors, 0, sizeof(allSensors));
//Taking names to the allocated list
    index = 0;

    while (dirEntry = readdir(dir))
    {
        if (dirEntry->d_type == DT_DIR)
        {
            if (strncmp(dirEntry->d_name, "28", strlen("28")) == 0)
            {
                allSensors[index] = dirEntry->d_name;
                allSensorAmountGlobal++;
                index++;
            }
        }
    }

    closedir(dir);
    return allSensors;
    //when to free allSensors
}

//funkcja nic nie zwraca, edytuje wejściową tablicę, nadpusjąc NULL-em niepodpięte czujki
char** GetPluggedSensorsName(char** allSensors)
{
    FILE* fd;
    long fileSize;
    char* buffer;
    char* path;
    char* textCursor;

    //int allSensorAmount = sizeof(allSensors)/sizeof(allSensors[0]); //should work

    printf("allSensorAmount = %d\n", allSensorAmountGlobal);

    //int pluggedSensorAmount = 0;

    for (int i = 0; i < allSensorAmountGlobal; i++)
    {
        path = calloc(64, sizeof(char));

        strncat(path, MAIN_DIR_PATH, strlen(MAIN_DIR_PATH)); //add /sys/...
        strncat(path, allSensors[i], strlen(allSensors[i])); // add /sys/.../28-00000...
        strncat(path, "/w1_slave", strlen("/w1_slave"));           // add /sys/.../28-00000.../w1_slave

        fd = fopen(path, "r");

        if (fd == NULL)
        {
            error("ERROR in opening some w1_slave file");
        }

        // checking file size
        fseek(fd, 0, SEEK_END);
        fileSize = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        buffer = calloc(fileSize, sizeof(char));
        fread(buffer, sizeof(char), fileSize, fd);
        textCursor = strstr(buffer, "t=");

        
        if (textCursor == NULL) //gives NULL if string is not founded
        {
            allSensors[i] = NULL; //delete name of the sensor from string array
        }
        else
        {
            pluggedSensorsAmountGlobal++;
        }

        free(path);
        free(buffer);
        fclose(fd);
    }

    printf("Plugged amount = %d\n", pluggedSensorsAmountGlobal);

//new list memory allocated
    char** onlyPluggedSensors = (char **)malloc(pluggedSensorsAmountGlobal * sizeof(char *));
    for (int i = 0; i < pluggedSensorsAmountGlobal; i++)
        onlyPluggedSensors[i] = (char *)malloc(20 * sizeof(char));

    memset(onlyPluggedSensors, 0, sizeof(onlyPluggedSensors));

//rewiritng list of sensors
    int j = 0;
    for (int i = 0; i < allSensorAmountGlobal; i++)
    {
        if (allSensors[i] != NULL)
        {
            onlyPluggedSensors[j] = allSensors[i];
            j++;
        }
    }
    //free(allSensors);
    return onlyPluggedSensors;
}




void TempReader(char** pluggedSensors)
{
    FILE* fd;
    char* buffer;
    char* path;
    char* textCursor;
    //int pluggedSensorAmount = sizeof(pluggedSensors)/sizeof(pluggedSensors[0]);
    long fileSize;
    float temperature;
    float tempArray[pluggedSensorsAmountGlobal];

    for (int i = 0; i < pluggedSensorsAmountGlobal; i++)
    {
        path = calloc(64, sizeof(char));

        strncat(path, MAIN_DIR_PATH, strlen(MAIN_DIR_PATH)); //add /sys/...
        strncat(path, pluggedSensors[i], strlen(pluggedSensors[i])); // add /sys/.../28-00000...
        strncat(path, "/w1_slave", strlen("/w1_slave"));           // add /sys/.../28-00000.../w1_slave

        fd = fopen(path, "r");

        if (fd == NULL)
        {
            error("ERROR in opening some w1_slave file");
        }

        // checking file size
        fseek(fd, 0, SEEK_END);
        fileSize = ftell(fd);
        fseek(fd, 0, SEEK_SET);

        buffer = calloc(fileSize, sizeof(char));
        fread(buffer, sizeof(char), fileSize, fd);
        textCursor = strstr(buffer, "t=");

        if (textCursor == NULL)
        {
            free(buffer);
            free(path);
            error("ERROR in temp finding");
        }
        textCursor += 2;
        temperature = atof(textCursor);
        temperature = temperature / 1000;

        tempArray[i] = temperature;
        printf("%s = %f\n", pluggedSensors[i], tempArray[i]);
        
        free(buffer);
        free(path);
        
        fclose(fd);
    }
}

void Display(char** pluggedSensors, float* tempArray)
{
    //int pluggedSensorAmount = sizeof(pluggedSensors)/sizeof(pluggedSensors[0]);

    for (size_t i = 0; i < pluggedSensorsAmountGlobal; i++)
    {
        printf("%s: %.2f\n", pluggedSensors[i], tempArray[i]);
    }
}


int main()
{
    DIR* dir;
    struct dirent* dirEntry;
    float* tempArray;
    char** allSensors;
    char** pluggedSensors;

    while (1)
    {    
        allSensors = GetAllSensorsName(dir, dirEntry);
        // for (size_t i = 0; i < allSensorAmountGlobal; i++)
        // {
        //      printf("%s\n", allSensors[i]);
        // }
        pluggedSensors = GetPluggedSensorsName(allSensors);
        free(allSensors);
        // for (size_t i = 0; i < pluggedSensorsAmountGlobal; i++)
        // {
        //      printf("%s\n", pluggedSensors[i]);
        // }
        TempReader(pluggedSensors);    
        free(pluggedSensors);

        allSensorAmountGlobal = 0;
        pluggedSensorsAmountGlobal = 0;
        sleep(2);
    }

    return 0;
}
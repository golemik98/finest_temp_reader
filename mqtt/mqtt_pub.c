#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

int rc;
struct mosquitto *mosq;


int flag = 1;

void flagSetter()
{
    puts("Quitting program");
    flag = 0;

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);

    mosquitto_lib_cleanup();    
}


int main()
{
    struct sigaction sa;
    sa.sa_handler = flagSetter;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);
    
    
       mosquitto_lib_init();
    mosq = mosquitto_new("Pub-test", true, NULL);

    rc = mosquitto_connect(mosq, "192.168.0.151", 1883, 60);

    if(rc != 0)
    {
        printf("Client could not connect to the broker! Error code: %d\n", rc);
        mosquitto_destroy(mosq);
        return -1;
    }

    printf("Connected to the broker!\n");

    int err = 0;
    int i = 0;
    char** tablica[] = {"Jestem john", "Jestem dave", "Jestem alex"};

    while (flag)
    {
        err = mosquitto_publish(mosq, NULL, "test/t1", 12, tablica[1], 0, false);

        if (err != MOSQ_ERR_SUCCESS)
        {
            puts("Error in publishing");
        }
         
        puts("Send");
        sleep(1);
    }

    puts("Program terminated correctly");
    return 0;
}



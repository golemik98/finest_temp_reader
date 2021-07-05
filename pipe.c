#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main()
{
    int fd[2];
    // fd[0] = read;
    // fd[1] = write;


    if (pipe(fd) == -1)
    {
        puts("An error occured with opening the pipe");
    }

    int id = fork();

    if (id == 0)
    {
        close(fd[0]); //because we do not read in this process
        int x;
        printf("Input a number: \n");
        scanf("%d", &x);
        write(fd[1], &x, sizeof(int));
        close(fd[1]);
    }
    else
    {
        close(fd[1]); //because we do not write in this process
        int y;
        read(fd[0], &y, sizeof(int));
        printf("Number is : %d\n", y);
        close(fd[0]);
    } 
}
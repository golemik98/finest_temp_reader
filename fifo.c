#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    if ((mkfifo("myfifo1", 0777) == -1))
    {
        puts("Error in creating fifo file");
        return 1;
    }

    puts("Opening ...");
    int fd = open("myfifo1", O_WRONLY);
    int x = 97;
    puts("Openned");

    puts("Writing ...");
    if (write("myfifo1", &x, sizeof(int)) == -1)
    {
        puts("Error in writing");
    }
    puts("Written");
    return 0;
}

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main()
{
    float temp = 22.2262222;
    char str[4];

    char* exm = "Ala ma kotaaaaaa";
    int x = sizeof(exm);
    printf("%d\n", x);
    gcvt(temp, 4, str);
    
    printf("%s\n", str);
    return 0;
}
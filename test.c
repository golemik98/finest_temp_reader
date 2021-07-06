#include <stdio.h>
#define MAX 100
  
int main()
{
    float x = 123.4567;
    char buf[MAX];
  
    gcvt(x, 6, buf);
  
    printf("buffer is: %s\n", buf);
  
    return 0;
}
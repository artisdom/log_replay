#include <stdio.h>

int main(int argc, char *argv[])
{
    int c;

    c = getchar();

    while (c != EOF) {
        printf("getchar: %d, 0x%x\n", c, c);
        //putchar(c);
        c = getchar();
    }

    return 0;
}

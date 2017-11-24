#include <stdio.h>
#include <curses.h>

int main(int argc, char *argv[])
{
    int c;

    c = getch();

    while (c != EOF) {
        printf("getchar: %d, 0x%x\n", c, c);
        //putchar(c);
        c = getch();
    }

    return 0;
}

// -lncurses

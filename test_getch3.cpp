#include <stdio.h>
#include <unistd.h>
#include <termios.h>

const int KEY_SPACE = 0x20;
const int KEY_f = 0x66;
const int KEY_b = 0x62;

const int KEY_F = 0x46;
const int KEY_B = 0x42;

char getch() {
    char buf = 0;
    struct termios old = {0};

    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");

    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");

    if (read(0, &buf, 1) < 0)
        perror ("read()");

    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");

    return (buf);
}

int main(int argc, char *argv[])
{
    int c;

    while ((c = getch()) != EOF) {
        printf("getchar: %d, 0x%x, '%c'\n", c, c, c);
        switch (c) {
        case KEY_SPACE:
            printf("space key pressed.\n");
            break;
        case KEY_F:
            printf("F key pressed.\n");
            break;
        case KEY_B:
            printf("B key pressed.\n");
            break;
        case KEY_f:
            printf("f key pressed.\n");
            break;
        case KEY_b:
            printf("b key pressed.\n");
            break;
        }
    }

    return 0;
}

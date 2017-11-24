#include <iostream>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <termios.h>

using namespace std;

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

int ACTION_PAUSED = 0;
int ACTION_SPEEDUP = 0;
//int ACTION_SLOWDOWN = 0;

int get_action()
{
    int c;

    while ((c = getch()) != EOF) {
        //printf("getchar: %d, 0x%x, '%c'\n", c, c, c);
        switch (c) {
        case KEY_SPACE:
            //printf("space key pressed.\n");
            if (ACTION_PAUSED == 0) {
                ACTION_PAUSED = 1;
            } else {
                ACTION_PAUSED = 0;
            }
            break;
        case KEY_F:
            printf("F key pressed.\n");
            ACTION_SPEEDUP++;
            break;
        case KEY_B:
            printf("B key pressed.\n");
            ACTION_SPEEDUP--;
            break;
        case KEY_f:
            printf("f key pressed.\n");
            ACTION_SPEEDUP++;
            break;
        case KEY_b:
            printf("b key pressed.\n");
            ACTION_SPEEDUP--;
            break;
        }
    }
}

int log_replay(char *filepath)
{
    ifstream filein(filepath);

    if (!filein.is_open()) {
        cout << "open file " << filepath << " failed.\n";
        return -1;
    }

    for (string line; std::getline(filein, line); )
    {
        while (ACTION_PAUSED) {
            usleep(100);
        }
        usleep(300000);
        cout << line << endl;
    }

    filein.close();

    return 0;
}

void usage(char *argv[]) {
    cout << argv[0] << " logfile" << endl;
    return;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        usage(argv);
        return -1;
    }

    std::thread t(log_replay, argv[1]);

    std::thread t2(get_action);
    t2.detach();

    t.join();
    //t2.join();

    return 0;
}

#include <iostream>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <boost/filesystem.hpp>
#include <boost/date_time.hpp>

using namespace std;
namespace fs = boost::filesystem;
namespace bt = boost::posix_time;

static struct termios orig_termios;  /* TERMinal I/O Structure */

int tty_reset(void)
{
    /* flush and reset */
    if (tcsetattr(0, TCSAFLUSH, &orig_termios) < 0)
        return -1;
    return 0;
}

/* exit handler for tty reset */
void tty_atexit(void)  /* NOTE: If the program terminates due to a signal   */
{                      /* this code will not run.  This is for exit()'s     */
    tty_reset();        /* only.  For resetting the terminal after a signal, */
}                      /* a signal handler which calls tty_reset is needed. */

const std::locale formats[] = {
    std::locale(std::locale::classic(), new bt::time_input_facet("%Y-%m-%d %H:%M:%S")),
    std::locale(std::locale::classic(), new bt::time_input_facet("%Y/%m/%d %H:%M:%S")),
    std::locale(std::locale::classic(), new bt::time_input_facet("%d.%m.%Y %H:%M:%S")),
    std::locale(std::locale::classic(), new bt::time_input_facet("%Y-%m-%d")),
    std::locale(std::locale::classic(), new bt::time_input_facet("%b %d %H:%M:%S%F")), //Nov 13 00:30:35.593
    std::locale(std::locale::classic(), new bt::time_input_facet("%b   %H:%M:%S%F")), //Nov 2 19:36:01.479 // day is only one digit, which boost doesn't support, use space to skip it.
};

const size_t formats_n = sizeof(formats) / sizeof(formats[0]);

const long MAX_INTERVIAL = 1000000; //1s
long MAX_INTERVIAL_CURRENT = MAX_INTERVIAL;
const long MAX_INTERVIAL_INCREASE_STEP = 300000; //300ms

const long DEFAULT_INTERVIAL = 300000; //300ms
const long DEFAULT_INTERVIAL_INCREASE_STEP = 100000; //100ms
long DEFAULT_INTERVIAL_CURRENT = DEFAULT_INTERVIAL;

bool get_ptime(const std::string &s, bt::ptime& pt)
{
    bool res = false;

    for (size_t i = 0; i < formats_n; ++i) {

        std::istringstream is(s);
        is.imbue(formats[i]);
        is >> pt;

        if (pt != bt::ptime()) {
            res = true;
            //std::cout << " ptime is " << pt << '\n';
            break;
        }
    }
    return res;
}

const int KEY_SPACE = ' '; // for pause
const int KEY_F = 'f';
const int KEY_B = 'b';
const int KEY_J = 'j'; // next line
const int KEY_N = 'n'; // next line
const int KEY_Q = 'q'; // quit
const int KEY_R = 'r'; // reset default interval
const int KEY_A = 'a'; // accelerate
const int KEY_D = 'd'; // decelerate

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
int ACTION_ACCELERATE = 0;
int ACTION_DECELERATE = 0;
int ACTION_NEXTLINE = 0;
int ACTION_QUIT = 0;
int ACTION_RESET = 0;

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
        case KEY_A:
            //printf("A key pressed.\n");
            ACTION_ACCELERATE++;
            if (MAX_INTERVIAL_CURRENT > MAX_INTERVIAL_INCREASE_STEP) {
                MAX_INTERVIAL_CURRENT -= MAX_INTERVIAL_INCREASE_STEP;
            }
            if (DEFAULT_INTERVIAL_CURRENT > DEFAULT_INTERVIAL_INCREASE_STEP) {
                DEFAULT_INTERVIAL_CURRENT -= DEFAULT_INTERVIAL_INCREASE_STEP;
            }
            break;
        case KEY_D:
            //printf("D key pressed.\n");
            ACTION_DECELERATE++;
            MAX_INTERVIAL_CURRENT += MAX_INTERVIAL_INCREASE_STEP;
            DEFAULT_INTERVIAL_CURRENT += DEFAULT_INTERVIAL_INCREASE_STEP;
            break;
        case KEY_R:
            //printf("R key pressed.\n");
            ACTION_RESET = 1;
            MAX_INTERVIAL_CURRENT = MAX_INTERVIAL;
            DEFAULT_INTERVIAL_CURRENT = DEFAULT_INTERVIAL;
            break;
        case KEY_J:
            ACTION_NEXTLINE++;
            break;
        case KEY_N:
            ACTION_NEXTLINE++;
            break;
        case KEY_Q:
            ACTION_QUIT = 1;;
            break;
        }
    }
}

bt::ptime last_ptime;

fs::path home_dir(getenv("HOME"));
const string CONFIG_DIR_NAME = ".log_replay";
fs::path config_dir = home_dir / CONFIG_DIR_NAME;

int log_replay(char *filepath)
{
    fs::path p(filepath);
    fs::path canon_p = canonical(p);

    string curr_config_file_name;
    ifstream filein(canon_p.string());
    streampos file_offset = 0;

    if (!filein.is_open()) {
        cout << "open file " << canon_p << " failed.\n";
        return -1;
    }

    for (const auto& dir : canon_p) {
        if (dir == "/") {
            continue;
        }
        curr_config_file_name += "_";
        curr_config_file_name += dir.string();
    }
    //cout << "curr_config_file_name " << curr_config_file_name << "\n";

    fs::path curr_config_file_path = config_dir / curr_config_file_name;

    if (fs::exists(curr_config_file_path))
    {
        //std::cout << "config file exists: " << curr_config_file_path << std::endl;
        ifstream curr_config_file(curr_config_file_path.string());
        string file_offset_str;
        std::getline(curr_config_file, file_offset_str);
        file_offset = stoi(file_offset_str);

        //cout << "file_offset_str: " << file_offset_str << "\n";
        //cout << "file_offset: " << file_offset << "\n";

        curr_config_file.close();

        filein.seekg(file_offset, ios_base::beg);
    }

    for (string line; std::getline(filein, line); )
    {
        bt::ptime pt;
        bool found_timestamp = get_ptime(line, pt);

        if (found_timestamp) {
            bt::time_duration td = pt - last_ptime;
            long microseconds = td.total_microseconds();

            last_ptime = pt;

            if (microseconds < 0) {
                microseconds = 0;
            } else if (microseconds > MAX_INTERVIAL_CURRENT) {
                microseconds = MAX_INTERVIAL_CURRENT;
            }

            //cout << "pt: " << pt << "\n";
            //cout << "microseconds: " << microseconds << "\n";

            usleep(microseconds);
        } else {
            usleep(DEFAULT_INTERVIAL_CURRENT);
        }

        cout << line << endl;

        if (ACTION_PAUSED) {
            while (ACTION_PAUSED) {
                if (ACTION_NEXTLINE) {
                    usleep(300000);
                    cout << line << endl;
                    ACTION_NEXTLINE--;
                    break;
                } else if (ACTION_QUIT) {
                    ACTION_PAUSED = 0;
                } else {
                    usleep(100);
                }
            }
        }

        if (ACTION_QUIT) {
            fstream f;
            f.open(curr_config_file_path.string(), std::fstream::out | std::fstream::trunc);
            f << filein.tellg();
            f.close();
            break;
        }
    }

    filein.close();

    return 0;
}

void usage(char *argv[]) {
    cout << argv[0] << " logfile" << endl;
    return;
}

int create_config_dir(void)
{
    if (!(fs::exists(config_dir))) {
        bool res = fs::create_directories(config_dir);
        if (res) {
            printf("create_directories succeed.\n");
        } else {
            printf("create_directories failed.\n");
        }
    } else {
        //printf("config folder exists.\n");
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        usage(argv);
        return -1;
    }

    /* store current tty settings in orig_termios */
    tcgetattr(0, &orig_termios);
    /* register the tty reset with the exit handler */
    atexit(tty_atexit);

    create_config_dir();

    std::thread t(log_replay, argv[1]);

    std::thread t2(get_action);
    t2.detach();

    t.join();
    //t2.join();

    return 0;
}

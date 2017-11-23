#include <iostream>
#include <fstream>
#include <unistd.h>

using namespace std;

int log_replay(char *filepath)
{
    ifstream filein(filepath);

    if (!filein.is_open()) {
        cout << "open file " << filepath << " failed.\n";
        return -1;
    }

    for (string line; std::getline(filein, line); )
    {
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

    log_replay(argv[1]);

    return 0;
}

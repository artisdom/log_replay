#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main() {
    // Get the current working directory
    fs::path cwd = fs::current_path();

    // Print the path to stdout
    std::cout << "generic: " << cwd.generic_string() << '\n';
    std::cout << "native: " << cwd.string() << '\n';
    std::cout << "quoted: " << cwd << '\n';
    std::cout << "Components: \n";

    for (const auto& dir : cwd) {
        std::cout <<'[' <<dir.string() << ']'; // each part
    }
    std::cout << '\n';
}

/*

  On Linux

  If you installed Boost libraries using your native package manager, then
  you can use the following commands to build your programs. Note that
  the library names are in system layout.

  $ g++ <source>.c -o <executable> -lboost_filesystem -lboost_system

  If you built Boost from source as shown in Chapter 1, Introducing Boost,
  and installed it under /opt/boost, you can use the following commands
  to compile and link your sources:

  $ g++ <source>.cpp -c -I/opt/boost/include
  $ g++ <source>.o -o <executable> -L/opt/boost/lib -lboost_filesystem-mt -lboost_system-mt -Wl,-rpath,/ opt/boost/lib

*/

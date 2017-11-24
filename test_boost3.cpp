#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main() {

    fs::path p("main.cpp");
    fs::path canon_p = canonical(p);

    std::cout << "generic: " << canon_p.generic_string() << '\n';
    std::cout << "native: " << canon_p.string() << '\n';
    std::cout << "quoted: " << canon_p << '\n';
    std::cout << "Components: \n";

    for (const auto& dir : canon_p) {
        std::cout <<'[' <<dir.string() << ']'; // each part
    }
    std::cout << '\n';
}
